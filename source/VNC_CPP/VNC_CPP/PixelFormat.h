#pragma once
#include "Common.h"
//#include <string.h>

/* 帧数据格式 */
struct PixelFormat {
    U8 bitPerPixel = 32;
    U8 depth = 24;
    U8 bigEndianFlag = 0;
    U8 trueColourFlag = 1;
    U16 redMax = 255;
    U16 greenMax = 255;
    U16 blueMax = 255;
    U8 redShift = 16;
    U8 greenShift = 8;
    U8 blueShift = 0;
    BYTE padding[3] = { 0 };
};

void pixelFormatToBuffer(PixelFormat pf, BYTE* buffer);

