﻿#pragma once
#include <boss.h>

#include "boss_integration_openh264-1.6.0.h"
#include <addon/openh264-1.6.0_for_boss/codec/api/svc/codec_api.h>

#include <boss.hpp>

class BaseEncoderH264
{
public:
    typedef void (*EncodeFrameCB)(const SFrameBSInfo& frameInfo);

public:
    BaseEncoderH264(sint32 width, sint32 height, bool fastmode);
    ~BaseEncoderH264();
    void EncodeTo(const uint32* rgba, id_flash flash, uint64 timems);

private:
    ISVCEncoder* mEncoder;
    uint08s mFrame;
    SSourcePicture mPic;
    SFrameBSInfo mInfo;
};