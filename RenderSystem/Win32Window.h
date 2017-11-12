//
// Created by shenc on 2017/11/7.
//

#ifndef SV_WIN32WINDOW_H
#define SV_WIN32WINDOW_H

#include "Prerequisites.h"
#include "Window.h"

class Win32Window : public Window
{
public:
    Win32Window(Device* device,const char* title,int x,int y,int w,int h, bool isFullScreen = false);
    virtual ~Win32Window();

private:
    virtual void _createSurface();

private:
    HWND m_pHwnd;
    HINSTANCE m_pHinstance;
};


#endif //SV_WIN32WINDOW_H
