//
// Created by shenc on 2017/10/28.
//

#include "VulkanSupport.h"
#include "PhysicalDevice.h"

template <> VulkanSupport* Singleton<VulkanSupport>::m_pSingleton = nullptr;

PFN_vkCreateDebugReportCallbackEXT CreateDebugReportCallback = VK_NULL_HANDLE;
PFN_vkDestroyDebugReportCallbackEXT DestroyDebugReportCallback = VK_NULL_HANDLE;
PFN_vkDebugReportMessageEXT dbgBreakCallback = VK_NULL_HANDLE;

VkBool32 _vkDebugReportCallbackEXT(
        VkDebugReportFlagsEXT                       flags,
        VkDebugReportObjectTypeEXT                  objectType,
        uint64_t                                    object,
        size_t                                      location,
        int32_t                                     messageCode,
        const char*                                 pLayerPrefix,
        const char*                                 pMessage,
        void*                                       pUserData)
{
    std::string errorStr = "Vulkan ";

    //消息类型
    if (flags & VK_DEBUG_REPORT_INFORMATION_BIT_EXT)
        errorStr += "Information:";
    else if (flags & VK_DEBUG_REPORT_WARNING_BIT_EXT)
        errorStr += "Waring:";
    else if (flags & VK_DEBUG_REPORT_ERROR_BIT_EXT)
        errorStr += "Error:";
    else if (flags & VK_DEBUG_REPORT_DEBUG_BIT_EXT)
        errorStr += "Debug:";
    else if (flags & VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT)
        errorStr += "Performance Waring:";

    //消息产生对象
    if (objectType == VK_DEBUG_REPORT_OBJECT_TYPE_UNKNOWN_EXT)
        errorStr += "Report Object Unknown";

    errorStr += pMessage;

#ifdef RS_DEBUG
    std::cout << errorStr.c_str() << std::endl;
#endif

    return true;
}

void _init_VulkanInstance(VkInstance& inst)
{
    std::vector<const char*> extensionNames;

    std::vector<const char*> layerNames;

    //检查当前SDK支持的层
    uint32_t EnumerateInstance = 0;
    std::vector<VkLayerProperties> layerProperties;
    vkEnumerateInstanceLayerProperties(&EnumerateInstance, nullptr);
    if (EnumerateInstance > 0)
    {
        layerProperties.resize(EnumerateInstance);
        vkEnumerateInstanceLayerProperties(&EnumerateInstance,layerProperties.data());

        for (int i = 0; i < EnumerateInstance ; ++i)
        {
            std::cout << layerProperties[i].layerName << std::endl;
            layerNames.push_back(layerProperties[i].layerName);
        }
    }

    //检查当前SDK支持的扩展
    std::vector<VkExtensionProperties> extensionProperties;
    vkEnumerateInstanceExtensionProperties(nullptr,&EnumerateInstance, nullptr);
    if (EnumerateInstance > 0)
    {
        extensionProperties.resize(EnumerateInstance);
        vkEnumerateInstanceExtensionProperties(nullptr,&EnumerateInstance, extensionProperties.data());

        for (int i = 0;i < EnumerateInstance ; ++i)
        {
            //加入所有支持的扩展
            extensionNames.push_back(extensionProperties[i].extensionName);
            std::cout << extensionProperties[i].extensionName << std::endl;
        }
    }

    VkApplicationInfo applicationInfo;
    applicationInfo.pNext = nullptr;
    applicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    applicationInfo.apiVersion = VK_API_VERSION_1_0;
    applicationInfo.pApplicationName = "SCL";
    applicationInfo.pEngineName = "SCL";
    applicationInfo.engineVersion = 0;
    applicationInfo.applicationVersion = 0;

    VkInstanceCreateInfo instanceCreateInfo;
    instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instanceCreateInfo.pNext = nullptr;
    instanceCreateInfo.flags = 0;
    instanceCreateInfo.pApplicationInfo = &applicationInfo;

    instanceCreateInfo.enabledExtensionCount = extensionNames.size();
    instanceCreateInfo.ppEnabledExtensionNames = extensionNames.data();

    instanceCreateInfo.enabledLayerCount = layerNames.size();
    instanceCreateInfo.ppEnabledLayerNames = layerNames.data();

    VkResult result = vkCreateInstance(&instanceCreateInfo, nullptr,&inst);
    if (result != VK_SUCCESS)
    {
        std::cout << "vkCreateInstance error Num:" << result << std::endl;
    }
}

void _setupDebugCallback(VkInstance& instance)
{
    CreateDebugReportCallback = reinterpret_cast<PFN_vkCreateDebugReportCallbackEXT>(
            vkGetInstanceProcAddr(instance,"vkCreateDebugReportCallbackEXT"));
    assert(CreateDebugReportCallback);

    DestroyDebugReportCallback =  reinterpret_cast<PFN_vkDestroyDebugReportCallbackEXT >(
            vkGetInstanceProcAddr(instance,"vkDestroyDebugReportCallbackEXT"));
    assert(DestroyDebugReportCallback);

    dbgBreakCallback = reinterpret_cast<PFN_vkDebugReportMessageEXT>(
            vkGetInstanceProcAddr(instance,"vkDebugReportMessageEXT"));
    assert(dbgBreakCallback);

    VkDebugReportCallbackEXT debugReportCallbackEXT;

    VkDebugReportCallbackCreateInfoEXT debugReportCallbackCreateInfoEXT = {};
    debugReportCallbackCreateInfoEXT.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
    debugReportCallbackCreateInfoEXT.pfnCallback = _vkDebugReportCallbackEXT;
    debugReportCallbackCreateInfoEXT.flags = VK_DEBUG_REPORT_INFORMATION_BIT_EXT|
            VK_DEBUG_REPORT_DEBUG_BIT_EXT|VK_DEBUG_REPORT_ERROR_BIT_EXT|
            VK_DEBUG_REPORT_WARNING_BIT_EXT|VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT;
    CreateDebugReportCallback(instance,&debugReportCallbackCreateInfoEXT, nullptr,&debugReportCallbackEXT);

    DestroyDebugReportCallback(instance,debugReportCallbackEXT, nullptr);
}

void _findPhysicalDevices(VkInstance& instance,std::vector<PhysicalDevice*>& physicalDevices)
{
    uint32_t count = 0;
    vkEnumeratePhysicalDevices(instance,&count, nullptr);
    if (count > 0)
    {
        std::vector<VkPhysicalDevice> _vkPhysicalDevices(count);
        vkEnumeratePhysicalDevices(instance,&count,_vkPhysicalDevices.data());

        for (auto& i:_vkPhysicalDevices)
        {
            PhysicalDevice* device = new PhysicalDevice(i);
            assert(device);
            physicalDevices.push_back(device);
        }
    }
}

VulkanSupport::VulkanSupport() :
        m_vulkanInstance(nullptr)
{
    m_PhysicalDevices.clear();

    _init_VulkanInstance(m_vulkanInstance);
    _setupDebugCallback(m_vulkanInstance);
    _findPhysicalDevices(m_vulkanInstance,m_PhysicalDevices);
}

VulkanSupport::~VulkanSupport()
{
    for (auto& i:m_PhysicalDevices)
    {
        delete i;
        i = nullptr;
    }
    m_PhysicalDevices.clear();

    if (m_vulkanInstance)
        vkDestroyInstance(m_vulkanInstance, nullptr);
}

VulkanSupport *VulkanSupport::getSingletonPrt()
{
    return m_pSingleton;
}

VulkanSupport &VulkanSupport::getSingleton()
{
    return *m_pSingleton;
}
