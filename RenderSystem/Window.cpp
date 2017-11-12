//
// Created by shenc on 2017/11/7.
//

#include "Window.h"
#include "VulkanSupport.h"
#include "PhysicalDevice.h"
#include "Device.h"


Window::Window(Device* device,const char *title, int x, int y, int w, int h, bool isFullScreen) :
        m_strWindowTitle(title),
        m_iWindowWidth(w),
        m_iWindowHeight(h),
        m_bIsFullScreen(isFullScreen),
        m_bIsClose(false),
        m_pSurface(VK_NULL_HANDLE),
        m_pDevice(device),
        m_pSwapchain(VK_NULL_HANDLE),
        m_pFormat(VK_FORMAT_B8G8R8A8_UNORM),
        m_iPresentQueueIndex(0)
{
}

Window::~Window()
{
    _uninit_swapchain();
}

void Window::_init_swapchain()
{
    //检查当前格式的表面是否能被绘制到屏幕
    const PhysicalDevice* physicalDevice = m_pDevice->getPhysicalDevice();

    int queueCount = physicalDevice->getQueueIndexCount();
    VkPhysicalDevice vkPhysicalDevice = physicalDevice->getVkPhysicalDevice();

    VkBool32* surfaceSuppor = (VkBool32*)malloc(sizeof(VkBool32)*queueCount);

    for (int i = 0; i < queueCount; ++i)
    {
        //获取每一个索引是否能被找到能够支持绘制到屏幕
        vkGetPhysicalDeviceSurfaceSupportKHR(vkPhysicalDevice,i,m_pSurface,&surfaceSuppor[i]);
    }

    int graphicsIndex = physicalDevice->getQueueIndexOfGraphics();
    //判断当前制图的索引是否能被渲染到表面
    if (surfaceSuppor[graphicsIndex]) //能支持
        m_iPresentQueueIndex = graphicsIndex;
    else //不能支持
    {
        for (int i = 0; i < queueCount; ++i)
        {
            if (surfaceSuppor[i] == VK_TRUE)
            {
                m_iPresentQueueIndex = i;
                break;
            }
        }
    }

    free(surfaceSuppor);

    //检查索引是否正确
    assert(m_iPresentQueueIndex < queueCount);

    //取得物理设备的支持当前绘图表面的支持格式
    std::vector<VkSurfaceFormatKHR> surfaceFormats;
    UINT32 sfcount = 0;

    vkGetPhysicalDeviceSurfaceFormatsKHR(vkPhysicalDevice,m_pSurface,&sfcount, nullptr);
    surfaceFormats.resize(sfcount);
    vkGetPhysicalDeviceSurfaceFormatsKHR(vkPhysicalDevice,m_pSurface,&sfcount,surfaceFormats.data());

    if (sfcount == 1 && surfaceFormats[0].format == VK_FORMAT_UNDEFINED)
    {
        //没有合适的表面格式
        m_pFormat = VK_FORMAT_B8G8R8A8_UNORM;
    } else if (sfcount > 0)
        m_pFormat = surfaceFormats[0].format;

    //取得表面的能力
    VkSurfaceCapabilitiesKHR surfaceCapabilitiesKHR;
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(vkPhysicalDevice,m_pSurface,&surfaceCapabilitiesKHR);

    //
    std::vector<VkPresentModeKHR> presentModes;
    uint32_t presentModeCount = 0;

    vkGetPhysicalDeviceSurfacePresentModesKHR(vkPhysicalDevice,m_pSurface,&presentModeCount, nullptr);
    presentModes.resize(presentModeCount);
    vkGetPhysicalDeviceSurfacePresentModesKHR(vkPhysicalDevice,m_pSurface,&presentModeCount, presentModes.data());

    VkExtent2D swapchainExtent;
    if (surfaceCapabilitiesKHR.currentExtent.width == 0xFFFFFFFF)
    {
        swapchainExtent.width = std::min((uint32_t)m_iWindowWidth,surfaceCapabilitiesKHR.maxImageExtent.width);
        swapchainExtent.height = std::max((uint32_t)m_iWindowHeight,surfaceCapabilitiesKHR.maxImageExtent.height);
    } else
        swapchainExtent = surfaceCapabilitiesKHR.currentExtent;

    VkSwapchainCreateInfoKHR swapchainCreateInfoKHR = {};
    swapchainCreateInfoKHR.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    swapchainCreateInfoKHR.pNext = nullptr;
    swapchainCreateInfoKHR.surface = m_pSurface;
    swapchainCreateInfoKHR.minImageCount = surfaceCapabilitiesKHR.minImageCount;

    swapchainCreateInfoKHR.imageFormat = m_pFormat;
    swapchainCreateInfoKHR.imageExtent.width = swapchainExtent.width;
    swapchainCreateInfoKHR.imageExtent.height = swapchainExtent.height;

    swapchainCreateInfoKHR.preTransform =
            (surfaceCapabilitiesKHR.supportedTransforms & VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR)?
            VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR:surfaceCapabilitiesKHR.currentTransform;
    swapchainCreateInfoKHR.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    swapchainCreateInfoKHR.imageArrayLayers = 1;

    swapchainCreateInfoKHR.presentMode = VK_PRESENT_MODE_FIFO_KHR;
    swapchainCreateInfoKHR.oldSwapchain = nullptr;
    swapchainCreateInfoKHR.clipped = true;

    swapchainCreateInfoKHR.imageColorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
    swapchainCreateInfoKHR.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    swapchainCreateInfoKHR.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;

    swapchainCreateInfoKHR.queueFamilyIndexCount = 0;
    swapchainCreateInfoKHR.pQueueFamilyIndices = nullptr;

    uint32_t queueFamilyIndices[2] = {(uint32_t)graphicsIndex,(uint32_t)m_iPresentQueueIndex};
    if (graphicsIndex != m_iPresentQueueIndex)
    {
        swapchainCreateInfoKHR.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        swapchainCreateInfoKHR.queueFamilyIndexCount = 2;
        swapchainCreateInfoKHR.pQueueFamilyIndices = queueFamilyIndices;
    }

    VkResult result = vkCreateSwapchainKHR(m_pDevice->getVulkanDevice(),&swapchainCreateInfoKHR, nullptr,&m_pSwapchain);
    if (result != VK_SUCCESS)
    {
#ifdef RS_DEBUG
        std::cout << "vkCreateSwapchainKHR Error" << std::endl;
#endif
    }

    uint32_t siCount = 0;
    vkGetSwapchainImagesKHR(m_pDevice->getVulkanDevice(),m_pSwapchain,&siCount, nullptr);
    m_pSwapchainImage.resize(siCount);
    m_pSwapchainImageView.resize(siCount);
    vkGetSwapchainImagesKHR(m_pDevice->getVulkanDevice(),m_pSwapchain,&siCount, m_pSwapchainImage.data());

    for (int j = 0; j < siCount; ++j) {
        VkImageViewCreateInfo imageViewCreateInfo = {};
        imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        imageViewCreateInfo.pNext = nullptr;
        imageViewCreateInfo.flags = 0;

        imageViewCreateInfo.image = m_pSwapchainImage[j];
        imageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        imageViewCreateInfo.format = m_pFormat;

        imageViewCreateInfo.components.a = VK_COMPONENT_SWIZZLE_A;
        imageViewCreateInfo.components.b = VK_COMPONENT_SWIZZLE_B;
        imageViewCreateInfo.components.g = VK_COMPONENT_SWIZZLE_G;
        imageViewCreateInfo.components.r = VK_COMPONENT_SWIZZLE_R;

        imageViewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
        imageViewCreateInfo.subresourceRange.layerCount = 1;
        imageViewCreateInfo.subresourceRange.baseMipLevel = 0;
        imageViewCreateInfo.subresourceRange.levelCount = 1;

        vkCreateImageView(m_pDevice->getVulkanDevice(),&imageViewCreateInfo, nullptr,&m_pSwapchainImageView[j]);
    }
}

void Window::_uninit_swapchain()
{
    if (m_pSwapchain)
    {
        VkDevice device = m_pDevice->getVulkanDevice();
        for (auto i :m_pSwapchainImageView)
        {
            vkDestroyImageView(device,i, nullptr);
        }
        m_pSwapchainImageView.clear();
        m_pSwapchainImage.clear();

        vkDestroySwapchainKHR(device,m_pSwapchain, nullptr);
        m_pSwapchain = nullptr;
    }

    if (m_pSurface)
    {
        VkInstance instance = VulkanSupport::getSingletonPrt()->getVkInstance();
        vkDestroySurfaceKHR(instance,m_pSurface, nullptr);
        m_pSurface = VK_NULL_HANDLE;
    }
}
