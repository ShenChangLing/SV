//
// Created by shenc on 2017/11/7.
//

#ifndef SV_WINDOW_H
#define SV_WINDOW_H

#include "Prerequisites.h"

class Window
{
public:
    Window(Device* device,const char* title,int x,int y,int w,int h, bool isFullScreen = false);
    virtual ~Window();

    //是现在在平台窗口类中实现
    static void messagePump();

    const char* getWindowTitle() const { return m_strWindowTitle.c_str(); }
    void getWindowTitle(std::string& str){ str = m_strWindowTitle; }


    bool getIsClose() { return m_bIsClose;}
    void setIsClose(bool isColse){ m_bIsClose = isColse;}

protected:
    virtual void _createSurface() = 0;
    virtual void _init_swapchain();
    virtual void _uninit_swapchain();

protected:
    std::string m_strWindowTitle;

    int m_iWindowWidth;
    int m_iWindowHeight;

    bool m_bIsFullScreen;

    //是否关闭
    bool m_bIsClose;

    //创建的设备
    Device* m_pDevice;

protected:
    VkSurfaceKHR m_pSurface;
    VkFormat m_pFormat;
    VkSwapchainKHR m_pSwapchain;
    std::vector<VkImage> m_pSwapchainImage;
    std::vector<VkImageView> m_pSwapchainImageView;

    //显示到表面的物理设备队列索引
    int m_iPresentQueueIndex;
};


#endif //SV_WINDOW_H
