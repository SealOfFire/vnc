#include <string.h>
# include "PixelFormat.h"

/**
 * 数据复制到byte数组中
 * start 开始的位置
 */
template<typename T> void CopyToArray(T value, unsigned char* dest, int start)
{
    int length = sizeof(T);
    unsigned char* temp = new unsigned char[length];
    memcpy(temp, &value, length);
    for (int i = 0; i < length; i++)
    {
        dest[start + i] = temp[length - 1 - i];
    }
    delete[] temp;
}

/**
 * byte数组中的数据转换成数值
 * start 开始的位置
 */
template<typename T> T ArrayToValue(unsigned char* dest, int start)
{
    int length = sizeof(T);
    unsigned char* temp = new unsigned char[length];
    for (int i = 0; i < length; i++)
    {
        temp[length - 1 - i] = dest[start + i];
    }
    T result = 0;
    memcpy(&result, temp, length);
    delete[] temp;
    return result;
}

void pixelFormatToBuffer(PixelFormat pf, BYTE* buffer) {
    //unsigned char buffer[16] = { 0 };
    buffer[0] = pf.bitPerPixel;
    buffer[1] = pf.depth;
    buffer[2] = pf.bigEndianFlag;
    buffer[3] = pf.trueColourFlag;
    CopyToArray(pf.redMax, buffer, 4);
    CopyToArray(pf.greenMax, buffer, 6);
    CopyToArray(pf.blueMax, buffer, 8);
    buffer[10] = pf.redShift;
    buffer[11] = pf.greenShift;
    buffer[12] = pf.blueShift;
    buffer[13] = pf.padding[0];
    buffer[14] = pf.padding[1];
    buffer[15] = pf.padding[2];
    //return buffer;
}