#pragma once

//#include <string.h>

struct pixelFormat {
    unsigned char bitPerPixel = 32;
    unsigned char depth = 24;
    unsigned char bigEndianFlag = 0;
    unsigned char trueColourFlag = 1;
    unsigned short redMax = 255;
    unsigned short greenMax = 255;
    unsigned short blueMax = 255;
    unsigned char redShift = 16;
    unsigned char greenShift = 8;
    unsigned char blueShift = 0;
    unsigned char padding[3] = { 0 };
};
template<typename T>
void CopyToArray(T value, unsigned char* dest, int destIndex)
{
    int length = sizeof(T);
    unsigned char* temp = new unsigned char[length];
    memcpy(temp, &value, length);
    for (int i = 0; i < length; i++)
    {
        dest[destIndex + i] = temp[length - 1 - i];
    }
    delete[] temp;
}
/*
void CopyToArray(unsigned short value, unsigned char* dest, int destIndex) {
    int length = 2;
    unsigned char temp[2];
    memcpy(temp, &value, length);
    for (int i = 0; i < length; i++)
    {
        dest[destIndex + i] = temp[length - 1 - i];
    }
}

void CopyToArray(unsigned int value, unsigned char* dest, int destIndex) {
    int length = 4;
    unsigned char temp[4];
    memcpy(temp, &value, length);
    for (int i = 0; i < length; i++)
    {
        dest[destIndex + i] = temp[length - 1 - i];
    }
}

void CopyToArray(int value, unsigned char* dest, int destIndex) {
    int length = 4;
    unsigned char temp[4];
    memcpy(temp, &value, length);
    for (int i = 0; i < length; i++)
    {
        dest[destIndex + i] = temp[length - 1 - i];
    }
}
*/
template<typename T>
T ArrayToValue(unsigned char* dest, int start) {
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

void pixelFormatToBuffer(pixelFormat pf, unsigned char* buffer) {
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

