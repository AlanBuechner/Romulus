#include "pch.h"
#include "VulkanSwapChain.h"
#include "Engine\Renderer\RendererCommand.h"
#include "VulkanRendererAPI.h"

#if defined(PLATFORM_WINDOWS)
#include "Platform\Windows\WindowsWindow.h"
#endif

namespace Engine
{

	VulkanSwapChain::VulkanSwapChain(uint32 width, uint32 height, void* window)
	{
		VulkanRendererAPI& api = *(VulkanRendererAPI*)RendererCommand::GetApiInstance();

#if defined(PLATFORM_WINDOWS)
		VkWin32SurfaceCreateInfoKHR createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
		createInfo.hinstance = GetModuleHandle(nullptr);
		createInfo.hwnd = ((WindowsWindow*)window)->GetHWND();

		CORE_ASSERT(vkCreateWin32SurfaceKHR(api.GetInstance(), &createInfo, nullptr, &m_WindowSurface) == VK_SUCCESS, 
			"Failded to create window surface");
#endif

		VkPhysicalDevice pDevice = api.GetPhysicalDevice();

		VkBool32 wsiSupported;
		vkGetPhysicalDeviceSurfaceSupportKHR(pDevice,
			api.GetDeviceQueueFamilyIndices(pDevice).graphicsFamily.value(),
			m_WindowSurface, &wsiSupported);
		CORE_ASSERT(wsiSupported, "wsi not supported");

		VkSurfaceCapabilitiesKHR surfaceCapabilities;
		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(pDevice, m_WindowSurface, &surfaceCapabilities);

		uint32 formatCount;
		vkGetPhysicalDeviceSurfaceFormatsKHR(pDevice, m_WindowSurface, &formatCount, nullptr);
		CORE_ASSERT(formatCount != 0, "no suface formats");

		std::vector< VkSurfaceFormatKHR> surfaceFormats{formatCount};
		vkGetPhysicalDeviceSurfaceFormatsKHR(pDevice, m_WindowSurface, &formatCount, surfaceFormats.data());
		if (surfaceFormats[0].format == VK_FORMAT_UNDEFINED)
		{
			m_SurfaceFormat.format = VK_FORMAT_B8G8R8A8_UNORM;
			m_SurfaceFormat.colorSpace = VK_COLORSPACE_SRGB_NONLINEAR_KHR;
		}
		else
			m_SurfaceFormat = surfaceFormats[0];




	}

	VulkanSwapChain::~VulkanSwapChain()
	{
		VulkanRendererAPI& api = *(VulkanRendererAPI*)RendererCommand::GetApiInstance();
		vkDestroySurfaceKHR(api.GetInstance(), m_WindowSurface, nullptr);
	}

	void VulkanSwapChain::Resize(uint32 width, uint32 height)
	{

	}

}