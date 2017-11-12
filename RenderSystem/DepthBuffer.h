//
// Created by Administrator on 2017/11/10.
//

#ifndef SV_DEPTHBUFFER_H
#define SV_DEPTHBUFFER_H

#include "Prerequisites.h"
#include "Buffer.h"

class DepthBuffer : public Buffer
{
public:
    DepthBuffer(Device *device,BufferType bufferType,int width,int height);

    ~DepthBuffer();

private:
    //深度影像
    VkImage m_pDepthImage;
    VkImageView m_pDepthImageView;
    VkDeviceMemory m_pDepthMemory;
};


#endif //SV_DEPTHBUFFER_H
