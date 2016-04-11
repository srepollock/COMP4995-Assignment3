#pragma once
#ifndef __BASICS
#define __BASICS
#define BITMAP_PICTURE "baboon.bmp"
#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080
#define BINFINITY FLT_MAX
#define EPSILON 0.001f
#include <windows.h>
#include <tchar.h>
#include <stdio.h>
void SetError(TCHAR*, ...);
HRESULT RestoreGraphics();
#endif