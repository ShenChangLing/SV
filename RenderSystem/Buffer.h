//
// Created by Administrator on 2017/11/11.
//

#ifndef SV_BUFFER_H
#define SV_BUFFER_H

#include "Prerequisites.h"

class Buffer
{
public:
<<<<<<< 6c970787d66b702c544730c7e9c7f638b127c9cd
    Buffer(Device* device);

    virtual ~Buffer();

protected:
    Device* m_pDevice;
=======
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
>>>>>>> be34319ba7319f2f83a014c8167252d5e121d724
};


#endif //SV_BUFFER_H
