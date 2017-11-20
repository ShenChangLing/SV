//
// Created by shenc on 2017/10/27.
//

#ifndef SV_RENDERSYSTEM_H
#define SV_RENDERSYSTEM_H

#include "Prerequisites.h"
#include "Singleton.h"

class RenderSystem : public Singleton<RenderSystem>
{
public:
    RenderSystem();

    virtual ~RenderSystem();

    //开启渲染循环
    void run();

private:
    VulkanSupport *m_pVulkanSupport;
    Device *m_pDevice;
    PipelineLayout *m_pPipelineLayout;
};

#endif //SV_RENDERSYSTEM_H
