//
// Created by Administrator on 2017/11/11.
//

#include "Buffer.h"

<<<<<<< 6c970787d66b702c544730c7e9c7f638b127c9cd
Buffer::Buffer(Device *device):
    m_pDevice(device)
=======
Buffer::Buffer(Device *device,BufferType bufferType):
    m_pDevice(device),
    m_eBufferType(bufferType)
>>>>>>> be34319ba7319f2f83a014c8167252d5e121d724
{
    //
}

Buffer::~Buffer()
{

}
