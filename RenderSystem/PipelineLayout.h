//
// Created by Administrator on 2017/11/20.
//

#ifndef SV_PIPELINELAYOUT_H
#define SV_PIPELINELAYOUT_H

#include "Prerequisites.h"

class PipelineLayout
{
public:
    PipelineLayout(Device* device, bool isUserTexture = false);
    ~PipelineLayout();

private:
    Device* m_pDevice;
    bool m_bIsUserTexture;

    VkDescriptorSetLayout m_pDescriptorSetLayout;
    VkPipelineLayout m_pPipelineLayout;

};


#endif //SV_PIPELINELAYOUT_H
