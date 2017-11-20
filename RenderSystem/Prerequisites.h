//
// Created by shenc on 2017/10/28.
//

#ifndef SV_PREREQUISITES_H
#define SV_PREREQUISITES_H

#ifndef RS_DEBUG
#define RS_DEBUG
#endif

#include <cassert>
#include <string>
#include <vector>
#include <list>
#include <deque>
#include <map>
#include <unordered_map>

#include <memory.h>
#include <iostream>

#include <array>

#include <algorithm>

#ifdef _WIN32
#ifndef VK_USE_PLATFORM_WIN32_KHR
#define VK_USE_PLATFORM_WIN32_KHR
#endif
#endif


#include <vulkan/vulkan.h>

class VulkanSupport;
class RenderSystem;
class PhysicalDevice;
class Device;

class Buffer;
class DepthBuffer;

class HardwareBuffer;
class HardwareBufferOfUniform;

class Pipeline;
class PipelineLayout;

class Window;

///////////////////////////

#include <iconv.h>

inline const char* utf8toGBK(const char* src)
{
    constexpr const int MaxOutChar = 1024;
    iconv_t cd = iconv_open("GBK","utf-8");

    //具体转换看这个
    static char bufout[MaxOutChar],*_sin,*_sout;

    _sin = (char*)src;
    _sout = bufout;

    size_t lenin = strlen(src)+1,lenout = MaxOutChar;
    if (-1 == iconv(cd,&_sin,&lenin,&_sout,&lenout))
    {
        //
        memset(bufout,0,MaxOutChar);
    }

    iconv_close(cd);
    return bufout;
}

#endif //SV_PREREQUISITES_H
