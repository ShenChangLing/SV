//
// Created by Administrator on 2017/11/10.
//

#include "DepthBuffer.h"
#include "Device.h"
#include "PhysicalDevice.h"

DepthBuffer::DepthBuffer(Device *device,BufferType bufferType,int width,int height):
    Buffer(device,bufferType),
    m_pDepthImage(VK_NULL_HANDLE),
    m_pDepthImageView(VK_NULL_HANDLE),
    m_pDepthMemory(VK_NULL_HANDLE)
{
    VkDevice vkDevice = m_pDevice->getVulkanDevice();

    constexpr static const VkFormat depth_format = VK_FORMAT_D16_UNORM;
    VkFormatProperties formatProperties = m_pDevice->getPhysicalDevice()->getFormatProperties(depth_format);

    VkImageCreateInfo imageCreateInfo = {};
    imageCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageCreateInfo.pNext = nullptr;

    if (formatProperties.linearTilingFeatures & VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT)
        imageCreateInfo.tiling = VK_IMAGE_TILING_LINEAR;
    else if (formatProperties.optimalTilingFeatures & VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT)
        imageCreateInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
    else
        imageCreateInfo.tiling = VK_IMAGE_TILING_LINEAR;

    imageCreateInfo.imageType = VK_IMAGE_TYPE_2D;
    imageCreateInfo.format = depth_format;

    imageCreateInfo.extent.width = width;
    imageCreateInfo.extent.height = height;
    imageCreateInfo.extent.depth = 1;

    imageCreateInfo.mipLevels = 1;
    imageCreateInfo.arrayLayers = 1;
    imageCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;

    imageCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    imageCreateInfo.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
    imageCreateInfo.queueFamilyIndexCount = 0;
    imageCreateInfo.pQueueFamilyIndices = nullptr;

    imageCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    imageCreateInfo.flags = 0;

    VkResult result = vkCreateImage(vkDevice,&imageCreateInfo, nullptr,&m_pDepthImage);
    if (result != VK_SUCCESS)
    {
#ifdef RS_DEBUG
        std::cout << "vkCreateImage error:" << result << std::endl;
#endif
    }

    assert(result == VK_SUCCESS);

    VkMemoryRequirements memoryRequirements = {};
    m_pDevice->getVkMemoryRequirements(m_pDepthImage,memoryRequirements);

    VkMemoryAllocateInfo memoryAllocateInfo = {};
    memoryAllocateInfo.pNext = nullptr;
    memoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    memoryAllocateInfo.allocationSize = memoryRequirements.size;

    if ((m_pDevice->getPhysicalDevice()->getMemoryTypeFormProperties(memoryRequirements.memoryTypeBits,0,&memoryAllocateInfo.memoryTypeIndex)) ==
            false)
    {
        //失败
#ifdef RS_DEBUG
        std::cout << __FILE__ << __LINE__ << __FUNCTION__ << "Error" << std::endl;
#endif
    }

    result = vkAllocateMemory(vkDevice,&memoryAllocateInfo, nullptr,&m_pDepthMemory);
    assert(result == VK_SUCCESS);

    vkBindImageMemory(vkDevice,m_pDepthImage,m_pDepthMemory,0);

    VkImageViewCreateInfo imageViewCreateInfo = {};
    imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    imageViewCreateInfo.pNext = nullptr;
    imageViewCreateInfo.format = depth_format;

    imageViewCreateInfo.image = m_pDepthImage;
    imageViewCreateInfo.components.r = VK_COMPONENT_SWIZZLE_R;
    imageViewCreateInfo.components.g = VK_COMPONENT_SWIZZLE_G;
    imageViewCreateInfo.components.b = VK_COMPONENT_SWIZZLE_B;
    imageViewCreateInfo.components.a = VK_COMPONENT_SWIZZLE_A;

    imageViewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
    imageViewCreateInfo.subresourceRange.baseMipLevel = 0;
    imageViewCreateInfo.subresourceRange.levelCount =1;
    imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
    imageViewCreateInfo.subresourceRange.layerCount = 1;

    imageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    imageViewCreateInfo.flags = 0;

    result = vkCreateImageView(vkDevice,&imageViewCreateInfo, nullptr,&m_pDepthImageView);
    assert(result ==  VK_SUCCESS);
}

DepthBuffer::~DepthBuffer()
{
    if (m_pDepthImageView)
    {
        vkDestroyImageView(m_pDevice->getVulkanDevice(),m_pDepthImageView, nullptr);
        m_pDepthImageView = VK_NULL_HANDLE;
    }
    if (m_pDepthImage)
    {
        vkDestroyImage(m_pDevice->getVulkanDevice(),m_pDepthImage, nullptr);
        m_pDepthImage = VK_NULL_HANDLE;
    }
    if (m_pDepthMemory)
    {
        vkFreeMemory(m_pDevice->getVulkanDevice(),m_pDepthMemory, nullptr);
        m_pDepthMemory = VK_NULL_HANDLE;
    }
}
