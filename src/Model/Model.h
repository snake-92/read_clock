#ifndef _CLASS_MODEL_H_
#define _CLASS_MODEL_H_

#include <opencv2/opencv.hpp>
#include <string>

enum TYPE_BAND
{
    SAME, // if hour and minute have the same angle
    HOUR,
    MINUTE
};

struct BANDS
{
    cv::Point p1;
    cv::Point p2;
    double angle;
    TYPE_BAND type;
};

/// @brief Class Model
class Model
{
public:
    Model();
    ~Model();

    std::vector<cv::Mat> ReadHour(const cv::Mat& image);
    cv::Mat DetectClock(const cv::Mat& image);
    std::string GetTime();

protected:
    cv::Mat Skeletonize(const cv::Mat& binary);
    std::vector<BANDS> FilterBandsByAngle(std::vector<BANDS>& lines, double angleGap = 2.0);
    void IdentifyBands(std::vector<BANDS>& bands, const cv::Point& center);
    double Distance(const cv::Point& p1, const cv::Point& p2);
    std::string GetMinute(const BANDS& band);
    std::string GetHours(const BANDS& band);
    
private:
    std::string m_time;
    cv::Mat m_imgOverlay;
    bool m_ReadHourFinished;
};

#endif