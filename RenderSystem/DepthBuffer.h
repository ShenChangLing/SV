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
<<<<<<< 6c970787d66b702c544730c7e9c7f638b127c9cd
    DepthBuffer(Device *device,int width,int height);
=======
    DepthBuffer(Device *device,BufferType bufferType,int width,int height);
>>>>>>> be34319ba7319f2f83a014c8167252d5e121d724
    ~DepthBuffer();

private:
    //深度影像
    VkImage m_pDepthImage;
    VkImageView m_pDepthImageView;
    VkDeviceMemory m_pDepthMemory;
};


#endif //SV_DEPTHBUFFER_H
