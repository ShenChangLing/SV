//
// Created by shenc on 2017/10/28.
//

#ifndef SV_VULKANSUPPORT_H
#define SV_VULKANSUPPORT_H

#include "Prerequisites.h"
#include "Singleton.h"


class VulkanSupport : public Singleton<VulkanSupport>
{
public:
    VulkanSupport();
    virtual ~VulkanSupport();

    static VulkanSupport* getSingletonPrt();
    static VulkanSupport& getSingleton();

    const VkInstance& getVkInstance() const { return m_vulkanInstance; }

    //取得第一个物理设备，如果存在的话
    PhysicalDevice* getPhysicalDeviceOfFirst() const
    {
        if (m_PhysicalDevices.size() > 0)
            return *m_PhysicalDevices.cbegin();

        return nullptr;
    }

private:
    //Vulkan 实例化对象
    VkInstance m_vulkanInstance;

    //当前运行设备的支持物理设备
    std::vector<PhysicalDevice*> m_PhysicalDevices;
};


#endif //SV_VULKANSUPPORT_H
