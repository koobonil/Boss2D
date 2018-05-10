#pragma once
#include <boss.h>

#include "boss_integration_opencv-3.1.0.h"
#include <addon/opencv-3.1.0_for_boss/include/opencv2/opencv.hpp>
extern "C"
{
    #include <addon/opencv-3.1.0_for_boss/include/opencv/cv.h>
    #include <addon/opencv-3.1.0_for_boss/include/opencv/highgui.h>
}

#include <boss.hpp>
#include <element/boss_point.hpp>
#include <element/boss_rect.hpp>

class CVObject
{
public:
    CVObject();
    ~CVObject();

public:
    // MOG2
    bool mEnableMOG2;
    sint32 mOldHistory;
    double mOldThreshold;
    bool mOldShadows;
    cv::Mat mMOG2Mask;
    cv::Mat mMOG2Image;
    cv::Ptr<cv::BackgroundSubtractor> mMOG2;
    // Canny
    bool mEnableCanny;
    double mLow;
    double mHigh;
    sint32 mAperture;
    cv::Mat mCannyImage;

    // Result
    cv::Mat mGrayImage;
    cv::Mat* mResult;
};
