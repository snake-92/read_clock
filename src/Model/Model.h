#ifndef _CLASS_MODEL_H_
#define _CLASS_MODEL_H_

#include <opencv2/opencv.hpp>

/// @brief Class Model
class Model
{
public:
    Model();
    ~Model();

    cv::Mat ReadHour(const cv::Mat& image);
    cv::Mat DetectClock(const cv::Mat& image);
};

#endif