// https://learn.microsoft.com/zh-cn/windows/win32/gdi/capturing-an-image
#pragma once

#include <boost/log/trivial.hpp>
//#include <wtypes.h>
//#include <Windows.h>
//#include <WinUser.h>
//#include <map>
#include "screen_win.h"
//#include <gdiplus.h>


HDC hScreenDC = GetDC(nullptr); // CreateDC("DISPLAY",nullptr,nullptr,nullptr);
HDC hMemoryDC = CreateCompatibleDC(hScreenDC);

std::map<unsigned int, unsigned char> keyMap = {
    { 0xff08, 0x08 }, // BackSpace
    { 0xff09, 0x09 }, // Tab
    { 0xff0d, 0x0d }, // Return or Enter
    { 0xff1b, 0x1b }, // Escape
    { 0xff63, 0x63 }, // Insert
    { 0xffff, 0xff }, // Delete
    { 0xff50, 0x50 }, // Home
    { 0xff51, 0x51 }, // Left
    { 0xff52, 0x52 }, // Up
    { 0xff53, 0x53 }, // Right
    { 0xff54, 0x54 }, // Down
    { 0xff55, 0x55 }, // Page Up
    { 0xff56, 0x56 }, // Page Down
    { 0xff57, 0x57 }, // End
    { 0xffbe, 0xbe }, // F1
    { 0xffbf, 0xbf }, // F2
    { 0xffc0, 0xc0 }, // F3
    { 0xffc1, 0xc1 }, // F4
    { 0xffc2, 0xc2 }, // F5
    { 0xffc3, 0xc3 }, // F6
    { 0xffc4, 0xc4 }, // F7
    { 0xffc5, 0xc5 }, // F8
    { 0xffc6, 0xc6 }, // F9
    { 0xffc7, 0xc7 }, // F10
    { 0xffc8, 0xc8 }, // F11
    { 0xffc9, 0xc9 }, // F12
    { 0xffe1, 0xA0 }, // Shift (left)
    { 0xffe2, 0xA1 }, // Shift (right)
    { 0xffe3, 0xA2 }, // Control (left)
    { 0xffe4, 0xA3}, // Control (right)
    { 0xffe7, 0xe7 }, // Meta (left)
    { 0xffe8, 0xe8 }, // Meta (right)
    { 0xffe9, 0xA4 }, // Alt (left)
    { 0xffea, 0xA5 }, // Alt (right)
    { 0x0041, 0x41 }, // A
    { 0x0061, 0x41 }, // a
    { 0x0042, 0x42 }, // B
    { 0x0062, 0x42 }, // b
    { 0x0043, 0x43 }, // C
    { 0x0063, 0x43 }, // c
    { 0x0044, 0x44 }, // D
    { 0x0064, 0x44 }, // d
    { 0x0045, 0x45 }, // E
    { 0x0065, 0x45 }, // e
    { 0x0046, 0x46 }, // F
    { 0x0066, 0x46 }, // f
    { 0x0047, 0x47 }, // G
    { 0x0067, 0x47 }, // g
    { 0x0048, 0x48 }, // H
    { 0x0068, 0x48 }, // h
    { 0x0049, 0x49 }, // I
    { 0x0069, 0x49 }, // i
    { 0x004A, 0x4A }, // J
    { 0x006A, 0x4A }, // j
    { 0x004B, 0x4B }, // K
    { 0x006B, 0x4B }, // k
    { 0x004C, 0x4C }, // L
    { 0x006C, 0x4C }, // l
    { 0x004D, 0x4D }, // M
    { 0x006D, 0x4D }, // m
    { 0x004E, 0x4E }, // N
    { 0x006E, 0x4E }, // n
    { 0x004F, 0x4F }, // O
    { 0x006F, 0x4F }, // o
    { 0x0050, 0x50 }, // P
    { 0x0070, 0x50 }, // p
    { 0x0051, 0x51 }, // Q
    { 0x0071, 0x51 }, // q
    { 0x0052, 0x52 }, // R
    { 0x0072, 0x52 }, // r
    { 0x0053, 0x53 }, // S
    { 0x0073, 0x53 }, // s
    { 0x0054, 0x54 }, // T
    { 0x0074, 0x54 }, // t
    { 0x0055, 0x55 }, // U
    { 0x0075, 0x55 }, // u
    { 0x0056, 0x56 }, // V
    { 0x0076, 0x56 }, // v
    { 0x0057, 0x57 }, // W
    { 0x0077, 0x57 }, // w
    { 0x0058, 0x58 }, // X
    { 0x0078, 0x58 }, // x
    { 0x0059, 0x59 }, // Y
    { 0x0079, 0x59 }, // y
    { 0x005A, 0x5A }, // Z
    { 0x007A, 0x5A }, // z
    { 0x0021, 0x31 }, // 1!
    { 0x0040, 0x32 }, // 2@
    { 0x0023, 0x33 },  // 3#
    { 0x0024, 0x34 }, // 4$
    { 0x0025, 0x35 }, // 5%
    { 0x005E, 0x36 }, // 6^
    { 0x0026, 0x37 }, // 7&
    { 0x002A, 0x38 }, // 8*
    { 0x0028, 0x39 }, // 9(
    { 0x0029, 0x30 }, // 0)
    { 0x002D, 0xBD }, // -
    { 0x005F, 0xBD }, // _
    { 0x002B, 0xBB }, // +
    { 0x003D, 0xBB }, // =
    { 0x0060, 0xC0 }, // `
    { 0x007E, 0xC0 }, // ~
    { 0x005B, 0xDB }, // [
    { 0x007B, 0xDB }, // {
    { 0x005D, 0xDD }, // ]
    { 0x007D, 0xDD }, // }
    { 0x005C, 0xDC }, // 反斜杠
    { 0x007C, 0xDC }, // |
    { 0x003A, 0xBA },// :
    { 0x003B, 0xBA },// ;
    { 0x0027, 0xDE }, //'
    { 0x0022, 0xDE },// "
    { 0x003C, 0xBC }, // <
    { 0x002C, 0xBC }, //,
    { 0x003E, 0xBE },// >
    { 0x002E, 0xBE },// .
    { 0x002F, 0xBF }, // /
    { 0x003F, 0xBF }, // ?
};


void bbb() {
    HDC hScreenDC = GetDC(nullptr); // CreateDC("DISPLAY",nullptr,nullptr,nullptr);
    HDC hMemoryDC = CreateCompatibleDC(hScreenDC);
    int width = GetDeviceCaps(hScreenDC, HORZRES);
    int height = GetDeviceCaps(hScreenDC, VERTRES);
    HBITMAP hBitmap = CreateCompatibleBitmap(hScreenDC, width, height);
    HBITMAP hOldBitmap = static_cast<HBITMAP>(SelectObject(hMemoryDC, hBitmap));
    BitBlt(hMemoryDC, 0, 0, width, height, hScreenDC, 0, 0, SRCCOPY);
    hBitmap = static_cast<HBITMAP>(SelectObject(hMemoryDC, hOldBitmap));
    DeleteDC(hMemoryDC);
    DeleteDC(hScreenDC);
}

unsigned short getWidth() {
    return GetDeviceCaps(hScreenDC, HORZRES);
}

unsigned short getHeight() {
    return GetDeviceCaps(hScreenDC, VERTRES);
}

/**
 * 屏幕截图
 */
void ScreenShort(int x, int y, int width, int height, unsigned char* data) {
    //HDC hScreenDC = GetDC(nullptr); // CreateDC("DISPLAY",nullptr,nullptr,nullptr);
    //HDC hMemoryDC = CreateCompatibleDC(hScreenDC);
    //int width = GetDeviceCaps(hScreenDC, HORZRES);
    //int height = GetDeviceCaps(hScreenDC, VERTRES);
    HBITMAP hBitmap = CreateCompatibleBitmap(hScreenDC, width, height);
    HBITMAP hOldBitmap = static_cast<HBITMAP>(SelectObject(hMemoryDC, hBitmap));
    BitBlt(hMemoryDC, x, y, width, height, hScreenDC, 0, 0, SRCCOPY);
    hBitmap = static_cast<HBITMAP>(SelectObject(hMemoryDC, hOldBitmap));

    //int iBmpWidth = hBitmap.bmWidth;
    //int iBmpHeight = hBitmap.bmHeight;
    int iBmpBitsPixel = 32;
    int iSize = width * width * (iBmpBitsPixel / 8);
    // unsigned char* data = new  unsigned char[iSize];
    GetBitmapBits(hBitmap, iSize, data);

    //DeleteDC(hMemoryDC);
    //DeleteDC(hScreenDC);
}

void GetScreenShot(void)
{
    int x1, y1, x2, y2, w, h;

    // get screen dimensions
    x1 = GetSystemMetrics(SM_XVIRTUALSCREEN);
    y1 = GetSystemMetrics(SM_YVIRTUALSCREEN);
    x2 = GetSystemMetrics(SM_CXVIRTUALSCREEN);
    y2 = GetSystemMetrics(SM_CYVIRTUALSCREEN);
    w = x2 - x1;
    h = y2 - y1;

    // copy screen to bitmap
    HDC     hScreen = GetDC(NULL);
    HDC     hDC = CreateCompatibleDC(hScreen);
    HBITMAP hBitmap = CreateCompatibleBitmap(hScreen, w, h);
    HGDIOBJ old_obj = SelectObject(hDC, hBitmap);
    BOOL    bRet = BitBlt(hDC, 0, 0, w, h, hScreen, x1, y1, SRCCOPY);

    // save bitmap to clipboard
    OpenClipboard(NULL);
    EmptyClipboard();
    SetClipboardData(CF_BITMAP, hBitmap);
    CloseClipboard();

    // clean up
    SelectObject(hDC, old_obj);
    DeleteDC(hDC);
    ReleaseDC(NULL, hScreen);
    DeleteObject(hBitmap);
}

// 记录鼠标按键按下状态
bool mouseButtonPress[8] = { 0 };

/**
 * 鼠标处理
 */
void pointerHandle(int* button, int x, int y) {
    BOOST_LOG_TRIVIAL(debug) << "pointerHandle begin";

    // 移动鼠标指针
    int result = SetCursorPos(x, y);

    // 鼠标按下
    for (int i = 0; i < 3; i++)
    {
        if (!mouseButtonPress[i] && button[i] == 1)
        {
            // 鼠标按下
            mouseButtonPress[i] = true;
            switch (i)
            {
            case 0:
                mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
                break;
            case 1:
                mouse_event(MOUSEEVENTF_MIDDLEDOWN, 0, 0, 0, 0);
                break;
            case 2:
                mouse_event(MOUSEEVENTF_RIGHTDOWN, 0, 0, 0, 0);
                break;
            default:
                break;
            }

        }
        if (mouseButtonPress[i] && button[i] == 1)
        {
            // 鼠标抬起
            mouseButtonPress[i] = false;
            switch (i)
            {
            case 0:
                mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
                break;
            case 1:
                mouse_event(MOUSEEVENTF_MIDDLEUP, 0, 0, 0, 0);
                break;
            case 2:
                mouse_event(MOUSEEVENTF_RIGHTUP, 0, 0, 0, 0);
                break;
            default:
                break;
            }
        }
    }
    BOOST_LOG_TRIVIAL(debug) << "pointerHandle end";
}

//std::variant<unsigned int, unsigned char> keyMap;
//keyMap

/**
 * 键盘输入处理
 */
void keyHandle(int downFlag, unsigned int key) {
    if (keyMap.count(key) > 0) {
        keybd_event(keyMap[key], 0, downFlag == 0 ? KEYEVENTF_KEYUP : 0, 0);
    }
    else {
        keybd_event(key, 0, downFlag == 0 ? KEYEVENTF_KEYUP : 0, 0);
    }
    //keybd_event()
}

/**
 * 释放资源
 */
void release() {

    DeleteDC(hMemoryDC);
    DeleteDC(hScreenDC);
}