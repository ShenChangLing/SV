//
// Created by Administrator on 2017/11/12.
//

#ifndef SV_HARDWAREBUFFER_H
#define SV_HARDWAREBUFFER_H

#include "Prerequisites.h"
#include "Buffer.h"

class HardwareBuffer : public Buffer
{
public:
    HardwareBuffer(Device* device,BufferType bufferType,uint32_t bufferSize);
    virtual ~HardwareBuffer();

    bool lock(int offset,int size);
    void unlock();

    void writeData(size_t offset,size_t length, const void* source);
    void readData(size_t offset,size_t length, void* dest);

protected:
    uint32_t m_iBufferSize;

    VkBuffer m_pBuffer;
    VkDeviceMemory m_pBufferMemory;
    //锁定后能读写的内存数据地址
    uint8_t* m_pBufferMemoryData;
};

#endif //SV_HARDWAREBUFFER_H
