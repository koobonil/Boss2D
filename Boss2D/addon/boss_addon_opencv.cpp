#include <boss.h>

#if !defined(BOSS_NEED_ADDON_OPENCV) || (BOSS_NEED_ADDON_OPENCV != 0 && BOSS_NEED_ADDON_OPENCV != 1)
    #error BOSS_NEED_ADDON_OPENCV macro is invalid use
#endif
bool __LINK_ADDON_OPENCV__() {return true;} // 링크옵션 /OPT:NOREF가 안되서 임시코드

#if BOSS_NEED_ADDON_OPENCV

#include "boss_addon_opencv.hpp"

#include <boss.hpp>
#include <format/boss_bmp.hpp>

// 등록과정
namespace BOSS
{
    BOSS_DECLARE_ADDON_FUNCTION(OpenCV, Create, id_opencv, void)
    BOSS_DECLARE_ADDON_FUNCTION(OpenCV, Release, void, id_opencv)
    BOSS_DECLARE_ADDON_FUNCTION(OpenCV, Update, void, id_opencv, id_bitmap_read, double, double, sint32)
    BOSS_DECLARE_ADDON_FUNCTION(OpenCV, GetPreprocessImage, id_bitmap, id_opencv)
    BOSS_DECLARE_ADDON_FUNCTION(OpenCV, TrackingObject, void, id_opencv)
    BOSS_DECLARE_ADDON_FUNCTION(OpenCV, GetObjectCount, sint32, id_opencv)
    BOSS_DECLARE_ADDON_FUNCTION(OpenCV, GetObjectRect, rect128f, id_opencv, sint32)
    BOSS_DECLARE_ADDON_FUNCTION(OpenCV, TrackingStick, void, id_opencv)
    BOSS_DECLARE_ADDON_FUNCTION(OpenCV, GetStickCount, sint32, id_opencv)
    BOSS_DECLARE_ADDON_FUNCTION(OpenCV, GetStickLine, vector128f, id_opencv, sint32)

    static autorun Bind_AddOn_OpenCV()
    {
        Core_AddOn_OpenCV_Create() = Customized_AddOn_OpenCV_Create;
        Core_AddOn_OpenCV_Release() = Customized_AddOn_OpenCV_Release;
        Core_AddOn_OpenCV_Update() = Customized_AddOn_OpenCV_Update;
        Core_AddOn_OpenCV_GetPreprocessImage() = Customized_AddOn_OpenCV_GetPreprocessImage;
        Core_AddOn_OpenCV_TrackingObject() = Customized_AddOn_OpenCV_TrackingObject;
        Core_AddOn_OpenCV_GetObjectCount() = Customized_AddOn_OpenCV_GetObjectCount;
        Core_AddOn_OpenCV_GetObjectRect() = Customized_AddOn_OpenCV_GetObjectRect;
        Core_AddOn_OpenCV_TrackingStick() = Customized_AddOn_OpenCV_TrackingStick;
        Core_AddOn_OpenCV_GetStickCount() = Customized_AddOn_OpenCV_GetStickCount;
        Core_AddOn_OpenCV_GetStickLine() = Customized_AddOn_OpenCV_GetStickLine;
        return true;
    }
    static autorun _ = Bind_AddOn_OpenCV();
}

// 구현부
namespace BOSS
{
    id_opencv Customized_AddOn_OpenCV_Create(void)
    {
        CVObject* NewObject = new CVObject();
        return (id_opencv) NewObject;
    }

    void Customized_AddOn_OpenCV_Release(id_opencv opencv)
    {
        CVObject* OldObject = (CVObject*) opencv;
        delete OldObject;
    }

    void Customized_AddOn_OpenCV_Update(id_opencv opencv, id_bitmap_read bmp, double low, double high, sint32 aperture)
    {
        if(!opencv) return;
        CVObject* CurObject = (CVObject*) opencv;
        cv::Mat OneImage(Bmp::GetHeight(bmp), Bmp::GetWidth(bmp), CV_8UC4, (void*) Bmp::GetBits(bmp));

        // 흑백화
        cv::Mat GrayImage;
        cv::cvtColor(OneImage, GrayImage, cv::COLOR_BGR2GRAY);

        // 캐니엣지화
        cv::blur(GrayImage, GrayImage, cv::Size(3, 3));
        cv::Canny(GrayImage, CurObject->mPreProcessImage, low, high, aperture);
    }

    id_bitmap Customized_AddOn_OpenCV_GetPreprocessImage(id_opencv opencv)
    {
        if(!opencv) return nullptr;
        CVObject* CurObject = (CVObject*) opencv;
        const cv::Mat& CurImage = CurObject->mPreProcessImage;

        auto NewBitmap = Bmp::Create(4, CurImage.cols, CurImage.rows);
        auto NewBitmapBits = (Bmp::bitmappixel*) Bmp::GetBits(NewBitmap);
        for(sint32 y = 0, yend = CurImage.rows; y < yend; ++y)
        {
            bytes Src = CurImage.data + CurImage.cols * y;
            auto Dst = &NewBitmapBits[CurImage.cols * y];
            for(sint32 x = 0, xend = CurImage.cols; x < xend; ++x)
            {
                Dst->b = *Src;
                Dst->g = *Src;
                Dst->r = *Src;
                Dst->a = 0xFF;
                Dst++; Src++;
            }
        }
        return NewBitmap;
    }

    void Customized_AddOn_OpenCV_TrackingObject(id_opencv opencv)
    {
        if(!opencv) return;
        CVObject* CurObject = (CVObject*) opencv;
        
        // 폐쇄도형수집
        std::vector< std::vector<cv::Point> > Contours;
        cv::findContours(CurObject->mPreProcessImage, Contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);

        // 소팅
        const sint32 BallMinSize = 4;
        const sint32 BallMaxSize = Math::Min(CurObject->mPreProcessImage.cols, CurObject->mPreProcessImage.rows) / 20;
        float BestScore = 0;
        Rect BestRect;
        for(size_t i = 0, iend = Contours.size(); i < iend; ++i)
        {
            bool IsSuccess = true;
            Rect BallRect(Contours[i][0].x, Contours[i][0].y, Contours[i][0].x, Contours[i][0].y);
            for(size_t j = 1, jend = Contours[i].size(); j < jend; ++j)
            {
                BallRect.l = Math::MinF(BallRect.l, Contours[i][j].x);
                BallRect.t = Math::MinF(BallRect.t, Contours[i][j].y);
                BallRect.r = Math::MaxF(BallRect.r, Contours[i][j].x);
                BallRect.b = Math::MaxF(BallRect.b, Contours[i][j].y);
                if(BallRect.t < CurObject->mPreProcessImage.rows * 3 / 4 || BallRect.l < CurObject->mPreProcessImage.cols * 1 / 5 ||
                    CurObject->mPreProcessImage.cols * 4 / 5 < BallRect.r) // 공의 위치한계
                {
                    IsSuccess = false;
                    break;
                }
                if(BallMaxSize < BallRect.Width() || BallMaxSize < BallRect.Height()) // 공크기 최대한계
                {
                    IsSuccess = false;
                    break;
                }
            }

            if(IsSuccess && BallMinSize < BallRect.Width() && BallMinSize < BallRect.Height()) // 공크기 최소한계
            {
                // 동그란 정도를 점수화
                point64f OPos = BallRect.Center();
                float MinDistance = Math::Distance(OPos.x, OPos.y, Contours[i][0].x, Contours[i][0].y);
                float MaxDistance = MinDistance;
                for(size_t j = 1, jend = Contours[i].size(); j < jend; ++j)
                {
                    const float OneDistance = Math::Distance(OPos.x, OPos.y, Contours[i][j].x, Contours[i][j].y);
                    MinDistance = Math::MinF(MinDistance, OneDistance);
                    MaxDistance = Math::MaxF(MaxDistance, OneDistance);
                }
                const float CurScore = MinDistance / MaxDistance;
                if(BestScore < CurScore)
                {
                    BestScore = CurScore;
                    BestRect = BallRect;
                }
            }
        }

        // 결과정리
        CurObject->mObjects.Clear();
        if(0.25f < BestScore) // 동그란 점수 최소치
        {
            const float MaxSize = Math::MaxF(BestRect.Width(), BestRect.Height());
            auto& NewRects = CurObject->mObjects.AtAdding();
            const float X = BestRect.CenterX();
            const float Y = BestRect.CenterY();
            const float R = MaxSize / 2;
            NewRects.l = X - R;
            NewRects.t = Y - R;
            NewRects.r = X + R;
            NewRects.b = Y + R;
        }
    }

    sint32 Customized_AddOn_OpenCV_GetObjectCount(id_opencv opencv)
    {
        if(!opencv) return 0;
        CVObject* CurObject = (CVObject*) opencv;
        return CurObject->mObjects.Count();
    }

    rect128f Customized_AddOn_OpenCV_GetObjectRect(id_opencv opencv, sint32 i)
    {
        if(!opencv) return Rect();
        CVObject* CurObject = (CVObject*) opencv;
        return CurObject->mObjects[i];
    }

    void Customized_AddOn_OpenCV_TrackingStick(id_opencv opencv)
    {
        if(!opencv) return;
        CVObject* CurObject = (CVObject*) opencv;

        const sint32 LineMinSize = Math::Min(CurObject->mPreProcessImage.cols, CurObject->mPreProcessImage.rows) / 4;
        const sint32 LineMaxGap = LineMinSize / 10;
        
        // 허프식 선분감지
        std::vector< cv::Vec4f > Lines;
        cv::HoughLinesP(CurObject->mPreProcessImage, Lines, 1, Math::ToRadian(1), 90, LineMinSize, LineMaxGap);

        // 선분수집
        CurObject->mSticks.Clear();
        for(size_t i = 0, iend = Lines.size(); i < iend; ++i)
        {
            const auto& CurLine = Lines[i];
            auto& NewStick = CurObject->mSticks.AtAdding();
            NewStick.x = CurLine[0];
            NewStick.y = CurLine[1];
            NewStick.vx = CurLine[2] - CurLine[0];
            NewStick.vy = CurLine[3] - CurLine[1];
        }
    }

    sint32 Customized_AddOn_OpenCV_GetStickCount(id_opencv opencv)
    {
        if(!opencv) return 0;
        CVObject* CurObject = (CVObject*) opencv;
        return CurObject->mSticks.Count();
    }

    vector128f Customized_AddOn_OpenCV_GetStickLine(id_opencv opencv, sint32 i)
    {
        if(!opencv) return Vector();
        CVObject* CurObject = (CVObject*) opencv;
        return CurObject->mSticks[i];
    }
}

CVObject::CVObject()
{
}

CVObject::~CVObject()
{
}

#endif
