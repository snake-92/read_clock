#include "Model.h"

Model::Model()
{

}

Model::~Model()
{
    
}


cv::Mat Model::ReadHour(const cv::Mat& image)
{
    cv::Mat imgNDG, imGauss, imThresh, imgSkelet, imFinal;

    // preprocessing
    cv::cvtColor(image, imgNDG, cv::COLOR_BGR2GRAY);
    cv::GaussianBlur(imgNDG, imGauss, cv::Size(3,3), 3);
    cv::threshold(imGauss, imThresh, 0, 255, cv::THRESH_BINARY_INV | cv::THRESH_OTSU);

    // skeletonize
    imgSkelet = Skeletonize(imThresh);

    // hough transform
    cv::cvtColor(imgSkelet, imFinal, cv::COLOR_BGR2RGB);
    std::vector<cv::Vec4i> lines;
    cv::HoughLinesP(imThresh, lines, 1, CV_PI/180, 80, 50, 10);
    
    for(const auto& line : lines)
    {
        cv::circle(imFinal, cv::Point(line[0], line[1]), 3, cv::Scalar(0,255,0), 2);
        cv::circle(imFinal, cv::Point(line[2], line[3]), 3, cv::Scalar(0,255,0), 2);
        cv::line( imFinal, cv::Point(line[0], line[1]), cv::Point(line[2], line[3]), cv::Scalar(0,0,255), 2, cv::LINE_AA);
    }

    return imFinal;
}


cv::Mat Model::DetectClock(const cv::Mat& image)
{
    cv::Mat img;
    cv::GaussianBlur(image, img, cv::Size(3,3), 0);
    return img;
}


/// @brief make a skeleton from a binary image
/// @param binary 
/// @return 
cv::Mat Model::Skeletonize(const cv::Mat& binary) 
{
    if(binary.type() != CV_8UC1)
    {
        throw std::invalid_argument("Image type must be CV_8UC1");
    }

    // Copy binary image and create skeleton image with the same size of binary
    cv::Mat skel = cv::Mat::zeros(binary.size(), CV_8UC1);
    cv::Mat img, temp, eroded;
    binary.copyTo(img);

    cv::Mat element = cv::getStructuringElement(cv::MORPH_CROSS, cv::Size(3, 3));

    bool done;
    do {
        // Erosion
        cv::erode(img, eroded, element);
        // Opening (dilate image eroded to get the contours)
        cv::dilate(eroded, temp, element);
        // substract edges
        cv::subtract(img, temp, temp);
        // Add to skeleton
        cv::bitwise_or(skel, temp, skel);
        // next iteration
        eroded.copyTo(img);

        done = (cv::countNonZero(img) == 0);
    } while (!done);

    return skel;
}