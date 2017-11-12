//
// Created by Administrator on 2017/11/11.
//

#ifndef SV_BUFFER_H
#define SV_BUFFER_H

#include "Prerequisites.h"

class Buffer
{
public:
    enum BufferType
    {
        BufferType_Depth,
        BufferType_Uniform,
    };
public:
    Buffer(Device* device,BufferType bufferType);

    virtual ~Buffer();

    BufferType getBufferType() const { return m_eBufferType; }

protected:
    Device* m_pDevice;
    BufferType m_eBufferType;
};


#endif //SV_BUFFER_H
