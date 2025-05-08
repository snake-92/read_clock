#include "Model.h"

Model::Model()
{

}

Model::~Model()
{
    
}


cv::Mat Model::ReadHour(const cv::Mat& image)
{
    cv::Mat img;
    cv::blur(image, img, cv::Size(3,3));
    return img;
}


cv::Mat Model::DetectClock(const cv::Mat& image)
{
    cv::Mat img;
    cv::GaussianBlur(image, img, cv::Size(3,3), 0);
    return img;
}