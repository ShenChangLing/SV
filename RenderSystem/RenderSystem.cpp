//
// Created by shenc on 2017/10/27.
//

#include "RenderSystem.h"
#include "VulkanSupport.h"
#include "Device.h"
#include "Window.h"


template <> RenderSystem* Singleton<RenderSystem>::m_pSingleton = nullptr;

RenderSystem::RenderSystem():
    m_pVulkanSupport(nullptr),
    m_pDevice(nullptr),
    m_pPipelineLayout(nullptr)
{
    m_pVulkanSupport = new VulkanSupport();

    m_pDevice = new Device(m_pVulkanSupport->getPhysicalDeviceOfFirst());
    m_pPipelineLayout = m_pDevice->createPipelineLayout(true);

}

RenderSystem::~RenderSystem()
{
    //
    if (m_pDevice)
    {
        if (m_pPipelineLayout)
        {
            m_pDevice->destroyPipelineLayout(m_pPipelineLayout);
            m_pPipelineLayout = nullptr;
        }
        delete m_pDevice;
        m_pDevice = nullptr;
    }

    if (m_pVulkanSupport)
    {
        delete m_pVulkanSupport;
        m_pVulkanSupport = nullptr;
    }
}

void RenderSystem::run()
{
    auto temp = m_pDevice->createWindow("hello",100,200,800,600);

    while (!temp->getIsClose())
    {
        Window::messagePump();
        Sleep(10);
    }
    m_pDevice->destoryWindow(temp);
}

