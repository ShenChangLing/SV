//
// Created by Administrator on 2017/11/12.
//

#ifndef SV_HARDWAREBUFFEROFUNIFORM_H
#define SV_HARDWAREBUFFEROFUNIFORM_H

#include "Prerequisites.h"
#include "HardwareBuffer.h"

class HardwareBufferOfUniform : public HardwareBuffer
{
public:
    HardwareBufferOfUniform(Device* device,BufferType bufferType,uint32_t bufferSize);

    virtual ~HardwareBufferOfUniform();
};


#endif //SV_HARDWAREBUFFEROFUNIFORM_H
