//
// Created by shenc on 2017/11/6.
//

#ifndef SV_PHYSICALDEVICE_H
#define SV_PHYSICALDEVICE_H

#include "Prerequisites.h"

//物理设备
class PhysicalDevice {
public:
    typedef std::vector<VkQueueFamilyProperties> QueueFamilyPropertiesList;
public:
    ~PhysicalDevice();
    PhysicalDevice(VkPhysicalDevice& device);

    const VkPhysicalDevice& getVkPhysicalDevice() const { return m_PhysicalDevice; }
    const QueueFamilyPropertiesList& getVkQueueFamilyProperties() const { return m_QueueFamilyProperties; }

    //找到绘图的队列索引编号
    //返回-1不存在
    int getQueueIndexOfGraphics() const;

    //取得队列的个数
    int getQueueIndexCount() const { return m_QueueFamilyProperties.size(); }

    //取得显示设备的格式属性
    VkFormatProperties getFormatProperties(VkFormat format);

    bool getMemoryTypeFormProperties(uint32_t typeBits,VkFlags requirements_mask, uint32_t *typeIndex);
protected:


private:
    VkPhysicalDevice m_PhysicalDevice;

    VkPhysicalDeviceProperties m_PhysicalDeviceProperties;
    VkPhysicalDeviceFeatures m_PhysicalDeviceFeatures;
    VkPhysicalDeviceMemoryProperties m_PhysicalDeviceMemoryProperties;

    QueueFamilyPropertiesList m_QueueFamilyProperties;
};


#endif //SV_PHYSICALDEVICE_H
