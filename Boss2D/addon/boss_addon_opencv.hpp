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
#include <element/boss_rect.hpp>
#include <element/boss_vector.hpp>

class CVObject
{
public:
    CVObject();
    ~CVObject();

public:
    cv::Mat mPreProcessImage;
    Rects mObjects;
    Vectors mSticks;
};
