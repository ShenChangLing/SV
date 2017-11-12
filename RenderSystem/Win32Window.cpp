//
// Created by shenc on 2017/11/7.
//

#include "Win32Window.h"
#include "VulkanSupport.h"
#include "Device.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

void Window::messagePump()
{
    MSG msg;
    while ( PeekMessage(&msg, nullptr,0,0,PM_REMOVE) )
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

LRESULT _win32_WNDPROC(HWND hwnd,UINT msg,WPARAM wparam,LPARAM lparam)
{
    if (msg == WM_CREATE)
    {
        //设置窗口标识
        SetWindowLongPtr(hwnd,GWLP_USERDATA,(LONG_PTR)((LPCREATESTRUCT)lparam)->lpCreateParams);
    }

    Win32Window* window = (Win32Window*)GetWindowLongPtr(hwnd,GWLP_USERDATA);
    if (!window)
    {//不是当前窗口的消息
        return DefWindowProc(hwnd,msg,wparam,lparam);
    }

    switch (msg)
    {
        case WM_CLOSE:
            window->setIsClose(true);
            return 0;
    }

    return DefWindowProc(hwnd,msg,wparam,lparam);
}

Win32Window::Win32Window(Device* device,const char *title, int x, int y, int w, int h, bool isFullScreen) :
        Window(device,title, x, y, w, h,isFullScreen),
        m_pHwnd(nullptr),
        m_pHinstance(nullptr)
{
    m_pHinstance = GetModuleHandle(NULL);

    constexpr const char *className = "VulkanWindows";

    WNDCLASSEX wndclassex = {};
    wndclassex.cbSize = sizeof(WNDCLASSEX);
    wndclassex.lpszClassName = className;

    wndclassex.cbClsExtra = 0;
    wndclassex.cbWndExtra = 0;

    wndclassex.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW | CS_GLOBALCLASS;
    wndclassex.lpfnWndProc = _win32_WNDPROC;

    wndclassex.hInstance = m_pHinstance;
    wndclassex.hCursor = LoadCursor(NULL,IDC_ARROW);
    wndclassex.hIconSm = (HICON)LoadImage(
            m_pHinstance,MAKEINTRESOURCE(5),IMAGE_ICON,
            GetSystemMetrics(SM_CXSMICON),GetSystemMetrics(SM_CYSMICON),LR_DEFAULTCOLOR);

    wndclassex.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);

    RegisterClassEx(&wndclassex);

    m_pHwnd = CreateWindowEx(WS_EX_OVERLAPPEDWINDOW,className,title,
                             WS_OVERLAPPEDWINDOW,x,y,w,h, nullptr, nullptr,m_pHinstance, this);

    UpdateWindow(m_pHwnd);
    ShowWindow(m_pHwnd,SW_SHOW);

    _createSurface();
    _init_swapchain();
}

Win32Window::~Win32Window()
{
    DestroyWindow(m_pHwnd);
    m_pHwnd = nullptr;
}

void Win32Window::_createSurface()
{
    VkInstance instance = VulkanSupport::getSingletonPrt()->getVkInstance();

    VkWin32SurfaceCreateInfoKHR win32SurfaceCreateInfoKHR = {};
    win32SurfaceCreateInfoKHR.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
    win32SurfaceCreateInfoKHR.pNext = nullptr;
    win32SurfaceCreateInfoKHR.hwnd = m_pHwnd;
    win32SurfaceCreateInfoKHR.hinstance = m_pHinstance;

    VkResult result = vkCreateWin32SurfaceKHR(instance,&win32SurfaceCreateInfoKHR, nullptr,&m_pSurface);
    if (result != VK_SUCCESS)
    {
#ifdef RS_DEBUG
        std::cout << "vkCreateWin32SurfaceKHR create error" << std::endl;
#endif
    }

}
