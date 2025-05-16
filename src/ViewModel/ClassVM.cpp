#include "ClassVM.h"


ClassVM::ClassVM(wxImage& image) : m_Image(image)
{
    m_model = std::make_shared<Model>();
    m_currentImgIndex = 0;
}

ClassVM::~ClassVM()
{

}

/// @brief Convert wxImage to cv::Mat
/// @param image_ 
/// @return 
cv::Mat ClassVM::ConvertWxImageToCvMat(wxImage& image_)
{
    // create cv::Mat image 3 channels 8 bits with the same size than the wxImage
    cv::Mat imagCV = cv::Mat::zeros(image_.GetHeight(), image_.GetWidth(), CV_8UC3);

    try
    {
        for(int y=0;y<image_.GetHeight();y++)
        {
            for(int x=0;x<image_.GetWidth();x++)
            {
                imagCV.at<cv::Vec3b>(y,x)[2] = image_.GetRed(x,y);
                imagCV.at<cv::Vec3b>(y,x)[1] = image_.GetGreen(x,y);
                imagCV.at<cv::Vec3b>(y,x)[0] = image_.GetBlue(x,y);
            }
        }
    }
    catch(const std::exception& e)
    {
        throw e.what();
    }

    return imagCV;
}


/// @brief convert cv::Mat to wxImage
/// @param image_ 
void ClassVM::ConvertCvMatToWxImage(const cv::Mat& image_)
{
    try
    {
        if(image_.channels()==1) // if 1 channel all values are put in the unique channel
        {
            for(int y=0;y<image_.rows;y++)
            {
                for(int x=0;x<image_.cols;x++)
                {
                    m_Image.SetRGB(x, y, image_.at<uchar>(y,x), image_.at<uchar>(y,x), image_.at<uchar>(y,x));
                }
            }
        }
        else
        {
            for(int y=0;y<image_.rows;y++)
            {
                for(int x=0;x<image_.cols;x++)
                {
                    //                                R                          G                              B
                    m_Image.SetRGB(x, y, image_.at<cv::Vec3b>(y,x)[2], image_.at<cv::Vec3b>(y,x)[1], image_.at<cv::Vec3b>(y,x)[0]);
                }
            }
        }
    }
    catch(const std::exception& e)
    {
        throw e.what();
    }
}


void ClassVM::NextImage()
{
    m_currentImgIndex++;

    // limits management 
    if(m_currentImgIndex >= m_listImg.size()) 
        m_currentImgIndex = 0;
    
    ConvertCvMatToWxImage(m_listImg[m_currentImgIndex]);
}


void ClassVM::PreviousImage()
{
    m_currentImgIndex--;

    // limits management 
    if(m_currentImgIndex < 0) 
        m_currentImgIndex = m_listImg.size() - 1;

    ConvertCvMatToWxImage(m_listImg[m_currentImgIndex]);
}


wxString ClassVM::ReadHour()
{
    try
    {
        m_listImg.clear();
        m_currentImgIndex = 0;
        m_listImg = m_model->ReadHour(ConvertWxImageToCvMat(m_Image));
        //ConvertCvMatToWxImage(m_listImg[m_currentImgIndex]);
    }
    catch(const std::exception& e)
    {
        throw e.what();
    }

    return (wxString)m_model->GetTime();
}

void ClassVM::DetectClock()
{
    try
    {
        ConvertCvMatToWxImage(m_model->DetectClock(ConvertWxImageToCvMat(m_Image)));   
    }
    catch(const std::exception& e)
    {
        throw e.what();
    }
}