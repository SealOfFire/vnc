// https://stackoverflow.com/questions/2607010/linux-how-to-capture-screen-and-simulate-mouse-movements
// https://stackoverflow.com/questions/14656615/xlib-xsendevent-click-event-do-not-work-inside-of-some-windows-on-ubuntu-12-04
// https://stackoverflow.com/questions/42013047/difference-between-xtestfakebuttonevent-xsendevent
#pragma once

#include "screen_linux.h"

// TODO 没有删除
Display* display = XOpenDisplay(NULL);
Window root = DefaultRootWindow(display);
XWindowAttributes gwa;

Status status = XGetWindowAttributes(display, root, &gwa);
int width22 = gwa.width;
int height22 = gwa.height;

unsigned short getWidth() {
    return gwa.width;
}

unsigned short getHeight() {
    return gwa.height;
}

void mouseClick(int button) {
    Display* display = XOpenDisplay(NULL);

    XEvent event;

    if (display == NULL) {
        fprintf(stderr, "Errore nell'apertura del Display !!!\n");
        exit(EXIT_FAILURE);
    }

    memset(&event, 0x00, sizeof(event));

    event.type = ButtonPress;
    event.xbutton.button = button;
    event.xbutton.same_screen = True;

    XQueryPointer(display, RootWindow(display, DefaultScreen(display)), &event.xbutton.root, &event.xbutton.window,
        &event.xbutton.x_root, &event.xbutton.y_root, &event.xbutton.x, &event.xbutton.y,
        &event.xbutton.state);

    event.xbutton.subwindow = event.xbutton.window;

    while (event.xbutton.subwindow) {
        event.xbutton.window = event.xbutton.subwindow;

        XQueryPointer(display, event.xbutton.window, &event.xbutton.root, &event.xbutton.subwindow,
            &event.xbutton.x_root, &event.xbutton.y_root, &event.xbutton.x, &event.xbutton.y,
            &event.xbutton.state);
    }

    if (XSendEvent(display, PointerWindow, True, 0xfff, &event) == 0)
        fprintf(stderr, "Errore nell'invio dell'evento !!!\n");

    XFlush(display);

    usleep(100000);

    event.type = ButtonRelease;
    event.xbutton.state = 0x100;

    if (XSendEvent(display, PointerWindow, True, 0xfff, &event) == 0)
        fprintf(stderr, "Errore nell'invio dell'evento !!!\n");

    XFlush(display);

    XCloseDisplay(display);
}

/**
 * 截屏处理
 */
void ScreenShort(int x, int y, int width, int height, unsigned char* data) {

    XImage* image = XGetImage(display, root,
        x, y,
        width, height,
        AllPlanes, ZPixmap);

    int size = width * height * 4;
    memcpy(data, image->data, size);

    int debug = 0;
    debug++;
    delete image;

}


// an xia de an jian
int mouseIsPress[8] = { 0 };

/**
 * 鼠标操作处理
 */
void pointerHandle(int* button, int x, int y) {
    // 移动光标
    XWarpPointer(display, None, root,
        0, 0, 0, 0, x, y);

    // 鼠标按钮
    bool hasEvent = false;
    int type = 0;
    int key = 0;
    for (int i = 0; i < 3; i++) {
        hasEvent = false;
        if (mouseIsPress[i] == 0 && button[i] == 1) {
            // shu biao an xia
            BOOST_LOG_TRIVIAL(debug) << "mouse key down";
            mouseIsPress[i] = button[i];
            key = i;
            type = ButtonPress;
            hasEvent = true;
            XTestFakeButtonEvent(display, i + 1, True, CurrentTime);
        }
        if (mouseIsPress[i] == 1 && button[i] == 0) {
            // shu biao tai qi
            BOOST_LOG_TRIVIAL(debug) << "mouse key up";
            key = i;
            mouseIsPress[i] = button[i];
            type = ButtonRelease;
            hasEvent = true;
            XTestFakeButtonEvent(display, i + 1, False, CurrentTime);
        }

    }

    XFlush(display);
}

/**
 * 键盘输入处理
 */
void keyHandle(int downFlag, unsigned int key) {

    //KeySym keysym = XK_X;
    KeyCode keycode = NoSymbol;
    //keycode = XKeysymToKeycode(display, keysym);
    keycode = XKeysymToKeycode(display, key);

    if (downFlag == 0) {
        // 抬起按键
        XTestFakeKeyEvent(display, keycode, False, 0);
    }
    else {
        // 按下按键
        XTestFakeKeyEvent(display, keycode, True, 0);
    }
    XFlush(display);
}

/**
 * 释放资源
 */
void release() {
    XFree(display);
}