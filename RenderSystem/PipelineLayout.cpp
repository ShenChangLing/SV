//
// Created by Administrator on 2017/11/20.
//

#include "PipelineLayout.h"

#include "Device.h"

PipelineLayout::PipelineLayout(Device *device, bool isUserTexture) :
        m_pDevice(device),
        m_bIsUserTexture(isUserTexture),
        m_pDescriptorSetLayout(VK_NULL_HANDLE),
        m_pPipelineLayout(VK_NULL_HANDLE)
{
    VkDescriptorSetLayoutBinding descriptorSetLayoutBinding[2];
    descriptorSetLayoutBinding[0].binding = 0;
    descriptorSetLayoutBinding[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    descriptorSetLayoutBinding[0].descriptorCount = 1;
    descriptorSetLayoutBinding[0].stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
    descriptorSetLayoutBinding[0].pImmutableSamplers = nullptr;

    if (m_bIsUserTexture)
    {
        descriptorSetLayoutBinding[1].binding = 1;
        descriptorSetLayoutBinding[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        descriptorSetLayoutBinding[1].descriptorCount = 1;
        descriptorSetLayoutBinding[1].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
        descriptorSetLayoutBinding[1].pImmutableSamplers = nullptr;
    }

    VkDescriptorSetLayoutCreateInfo descriptorSetLayoutCreateInfo = {};
    descriptorSetLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    descriptorSetLayoutCreateInfo.pNext = nullptr;
    descriptorSetLayoutCreateInfo.flags = 0;
    descriptorSetLayoutCreateInfo.bindingCount = m_bIsUserTexture?2:1;
    descriptorSetLayoutCreateInfo.pBindings = descriptorSetLayoutBinding;

    VkResult result = vkCreateDescriptorSetLayout(
            m_pDevice->getVulkanDevice(),&descriptorSetLayoutCreateInfo, nullptr,&m_pDescriptorSetLayout);
    assert(result == VK_SUCCESS);

    VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo = {};
    pipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutCreateInfo.pNext = nullptr;
    pipelineLayoutCreateInfo.pushConstantRangeCount = 0;
    pipelineLayoutCreateInfo.pPushConstantRanges = nullptr;
    pipelineLayoutCreateInfo.setLayoutCount = 1;
    pipelineLayoutCreateInfo.pSetLayouts = &m_pDescriptorSetLayout;

    result = vkCreatePipelineLayout(m_pDevice->getVulkanDevice(),&pipelineLayoutCreateInfo, nullptr,&m_pPipelineLayout);
    assert(result == VK_SUCCESS);
}

PipelineLayout::~PipelineLayout()
{
    if (m_pPipelineLayout)
    {
        vkDestroyPipelineLayout(m_pDevice->getVulkanDevice(),m_pPipelineLayout, nullptr);
        m_pPipelineLayout = VK_NULL_HANDLE;
    }
    if (m_pDescriptorSetLayout)
    {
        vkDestroyDescriptorSetLayout(m_pDevice->getVulkanDevice(),m_pDescriptorSetLayout, nullptr);
        m_pDescriptorSetLayout = VK_NULL_HANDLE;
    }

    m_pDevice = nullptr;
}
