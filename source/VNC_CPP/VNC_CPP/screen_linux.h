#include <X11/Xlib.h>
#include <X11/X.h>
#include <X11/keysym.h>
#include <X11/extensions/XTest.h>
#include "logger.h"

unsigned short getWidth();
unsigned short getHeight();

void ScreenShort(int x, int y, int width, int height, unsigned char* data);
void mouseClick(int button);
void pointerHandle(int* button, int x, int y);
void keyHandle(int downFlag, unsigned int key);
void release();