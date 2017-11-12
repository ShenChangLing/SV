//
// Created by shenc on 2017/11/6.
//

#include "PhysicalDevice.h"

const char* _PhysicalDeviceType2Char(VkPhysicalDeviceType type)
{
    std::string typeSrc ="当前显卡的类型为: ";
    switch (type)
    {
        case VK_PHYSICAL_DEVICE_TYPE_OTHER:
            typeSrc += "其他";
            break;
        case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:
            typeSrc += "独立";
            break;
        case VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU:
            typeSrc += "虚拟";
            break;
        case VK_PHYSICAL_DEVICE_TYPE_CPU:
            typeSrc += "CPU集成";
            break;
        default:
            typeSrc += "错误类型";
            break;
    }
    return utf8toGBK(typeSrc.c_str());
}

void _OutputDeviceProperties(const VkPhysicalDeviceProperties& properties)
{
    std::cout << utf8toGBK("显卡名称: ") << properties.deviceName << "\n"
            << "vendorID: " << properties.vendorID << "\n"
            << "driverVersion: " << VK_VERSION_MAJOR(properties.driverVersion) << "." << VK_VERSION_MINOR(properties.driverVersion) << "." << VK_VERSION_PATCH(properties.driverVersion) << "\n"
            << "deviceID: " << properties.deviceID << "\n"
            << "apiVersion: " << VK_VERSION_MAJOR(properties.apiVersion) << "." << VK_VERSION_MINOR(properties.apiVersion) << "." << VK_VERSION_PATCH(properties.apiVersion) << "\n"
            << _PhysicalDeviceType2Char(properties.deviceType) << "\n";
}


PhysicalDevice::PhysicalDevice(VkPhysicalDevice& device):
    m_PhysicalDevice(device)
{
    assert(m_PhysicalDevice);

    vkGetPhysicalDeviceProperties(m_PhysicalDevice,&m_PhysicalDeviceProperties);
    vkGetPhysicalDeviceFeatures(m_PhysicalDevice,&m_PhysicalDeviceFeatures);

#ifdef RS_DEBUG
    _OutputDeviceProperties(m_PhysicalDeviceProperties);
#endif

    uint32_t count = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(m_PhysicalDevice,&count, nullptr);
    if (count > 0)
    {
        m_QueueFamilyProperties.resize(count);

        vkGetPhysicalDeviceQueueFamilyProperties(m_PhysicalDevice,&count, m_QueueFamilyProperties.data());
    }

    vkGetPhysicalDeviceMemoryProperties(m_PhysicalDevice,&m_PhysicalDeviceMemoryProperties);

}

PhysicalDevice::~PhysicalDevice()
{
    m_PhysicalDevice = VK_NULL_HANDLE;
    m_QueueFamilyProperties.clear();
}

int PhysicalDevice::getQueueIndexOfGraphics() const
{
    int count = m_QueueFamilyProperties.size();

    int findIndex = -1;

    for (int i = 0; i < count; ++i)
    {
        //开始查找
        if (m_QueueFamilyProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
            findIndex = i;
    }
    return findIndex;
}

VkFormatProperties PhysicalDevice::getFormatProperties(VkFormat format)
{
    VkFormatProperties formatProperties;

    vkGetPhysicalDeviceFormatProperties(m_PhysicalDevice,format,&formatProperties);

    return formatProperties;
}

bool PhysicalDevice::getMemoryTypeFormProperties(uint32_t typeBits, VkFlags requirements_mask, uint32_t *typeIndex)
{
    for (int i = 0; i < m_PhysicalDeviceMemoryProperties.memoryTypeCount; ++i)
    {
        if ((typeBits & 1) == 1)
        {
            if ((m_PhysicalDeviceMemoryProperties.memoryTypes[i].propertyFlags & requirements_mask) == requirements_mask)
            {
                *typeIndex = i;
                return true;
            }
        }
        typeBits >>= 1;
    }
    return false;
}
