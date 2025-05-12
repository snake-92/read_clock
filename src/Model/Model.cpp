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

    // erode image
    cv::Mat element = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5));
    cv::erode(imThresh, imThresh, element);

    // separate two lines in the image center
    cv::Point center = cv::Point(imThresh.cols/2, imThresh.rows/2);
    cv::circle(imThresh, center, 25, cv::Scalar(0,0,0), -1);
   
    // skeletonize
    imgSkelet = Skeletonize(imThresh);
    
    // hough transform
    cv::cvtColor(imgSkelet, imFinal, cv::COLOR_BGR2RGB);
    std::vector<cv::Vec4i> lines;
    cv::HoughLinesP(imgSkelet, lines, 1, CV_PI/180, 20, 5, 10);
    
    // dectect bands
    std::vector<BANDS> bands;

    // permit to detect bands only in a certain distance
    // the image has a same number of rows and cols
    int distMin = (imThresh.cols/2)-30;

    for(const auto& line : lines)
    {
        cv::Point p1 = cv::Point(line[0], line[1]);
        cv::Point p2 = cv::Point(line[2], line[3]);

        // distance with center
        double d0 = Distance(p1, center);
        double d2 = Distance(p2, center);

        // p1 is always nearest to center
        if(d0 > d2)
        {
            std::swap(p1, p2);
        }

        d0 = std::sqrt(std::pow(p1.x - center.x, 2) + std::pow(p1.y - center.y, 2));
        // verify the position of the line. Permit to know if it is the band or not
        // we use p1
        if(d0 > distMin)
        {
            continue;
        }

        // determine line angle
        double angle = -1*(std::atan2(p2.y - p1.y, p2.x - p1.x) * 180 / CV_PI);
        if(angle < 0)
        {
            angle += 360;
        }

        bands.push_back(BANDS{p1, p2, angle});
    }

    // delete all line with the same angle
    bands = FilterBandsByAngle(bands, 3.0);
    IdentifyBands(bands, center);

    // draw bands
    for(const auto& band : bands)
    {
        cv::line(imFinal, band.p1, band.p2, cv::Scalar(255,0,0), 2, cv::LINE_AA);
        cv::circle(imFinal, band.p1, 3, cv::Scalar(0,255,0), 2);
        cv::circle(imFinal, band.p2, 3, cv::Scalar(0,255,0), 2);

        std::string text = std::to_string((int)band.angle);
        cv::putText(imFinal, text, band.p1, cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255,0,255), 1);

        if(band.type == TYPE_BAND::HOUR)
        {
            cv::putText(imFinal, "HOUR", band.p2, cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255,0,255), 1);
        }
        else if(band.type == TYPE_BAND::MINUTE)
        {
            cv::putText(imFinal, "MINUTE", band.p2, cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255,0,255), 1);
            cv::putText(imFinal, GetMinute(band) , cv::Point(5,15), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255,0,255), 1);
        }
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


/// @brief Distance between two points
/// @param p1 
/// @param p2 
/// @return 
double Model::Distance(const cv::Point& p1, const cv::Point& p2)
{
    return std::sqrt(std::pow(p1.x - p2.x, 2) + std::pow(p1.y - p2.y, 2));
}


/// @brief Delete lines with the same angle or too close
/// @param lines 
/// @param angleGap 
/// @return 
std::vector<BANDS> Model::FilterBandsByAngle(std::vector<BANDS>& lines, double angleGap/* = 2.0*/) 
{
    if(lines.empty())
    {
        throw std::invalid_argument("No lines found. (FilterBandsByAngle)");
    }

    std::vector<BANDS> result;

    // Sort lines by angle
    std::sort(lines.begin(), lines.end(), [](const BANDS& a, const BANDS& b) { return a.angle < b.angle; });

    for (const auto& line : lines) {
        // Verify if angle is too close
        bool tooClose = false;
        for (const auto& kept : result) {
            if (std::abs(line.angle - kept.angle) < angleGap) {
                tooClose = true;
                break;
            }
        }
        if (!tooClose) 
        {
            result.push_back(line);
        }
    }

    return result;
}


/// @brief Identify type of bands minute or hour
/// @param bands 
void Model::IdentifyBands(std::vector<BANDS>& bands, const cv::Point& center) 
{
    if(bands.empty())
    {
        throw std::invalid_argument("No bands found. (IdentifyBands)");
    }

    if(bands.size() == 1)
    {
        bands[0].type = TYPE_BAND::SAME;
    }
    else if(bands.size() == 2)
    {
        bands[0].type = TYPE_BAND::HOUR;
        bands[1].type = TYPE_BAND::MINUTE;
        
        if(Distance(bands[0].p2, center) > Distance(bands[1].p2, center))
        {
             bands[1].type = TYPE_BAND::HOUR;
             bands[0].type = TYPE_BAND::MINUTE;
        }
    }
    else
    {
        throw std::invalid_argument("Too many bands found. (IdentifyBands)");    
    }
}


std::string Model::GetMinute(const BANDS& band)
{
    if(band.type == TYPE_BAND::HOUR)
    {
        throw std::invalid_argument("Band is not minute. (GetMinute)");
    }

    // get minute by value of angle
    int gap = 5;
    if(band.angle <= gap || band.angle >= 360-gap)
    {
        return "15";
    }
    else if(band.angle <= 30+gap && band.angle >= 30-gap)
    {
        return "10";
    }
    else if(band.angle <= 60+gap && band.angle >= 60-gap)
    {
        return "05";
    }
    else if(band.angle <= 90+gap && band.angle >= 90-gap)
    {
        return "00";
    }
    else if(band.angle <= 120+gap && band.angle >= 120-gap)
    {
        return "55";
    }
    else if(band.angle <= 150+gap && band.angle >= 150-gap)
    {
        return "50";
    }
    else if(band.angle <= 180+gap && band.angle >= 180-gap)
    {
        return "45";
    }
    else if(band.angle <= 210+gap && band.angle >= 210-gap)
    {
        return "40";
    }
    else if(band.angle <= 240+gap && band.angle >= 240-gap)
    {
        return "35";
    }
    else if(band.angle <= 270+gap && band.angle >= 270-gap)
    {
        return "30";
    }
    else if(band.angle <= 300+gap && band.angle >= 300-gap)
    {
        return "25";
    }
    else if(band.angle <= 330+gap && band.angle >= 330-gap)
    {
        return "20";
    }

    return "XX";
}