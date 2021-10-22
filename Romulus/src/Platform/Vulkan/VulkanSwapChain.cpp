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
		WindowsWindow& win = *(WindowsWindow*)window;

		VkWin32SurfaceCreateInfoKHR createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
		createInfo.hinstance = GetModuleHandle(nullptr);
		createInfo.hwnd = win.GetHWND();

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
		if (surfaceCapabilities.currentExtent.width < UINT32_MAX)
		{
			CORE_ASSERT(surfaceCapabilities.currentExtent.width == width && surfaceCapabilities.currentExtent.height == height,
				"Surface width and height dont match width and height provided");
		}

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

		// determine the number of swapchain images
		uint32 bufferCount = 2;
		if (surfaceCapabilities.minImageCount <= 3 && surfaceCapabilities.maxImageCount >= 3)
			bufferCount = 3;

		// find best presentation mode
		VkPresentModeKHR presentMode = VK_PRESENT_MODE_FIFO_KHR;
		uint32 presentModeCount = 0;
		vkGetPhysicalDeviceSurfacePresentModesKHR(api.GetPhysicalDevice(), m_WindowSurface, &presentModeCount, nullptr);
		std::vector<VkPresentModeKHR> presentmodes{ presentModeCount };
		vkGetPhysicalDeviceSurfacePresentModesKHR(api.GetPhysicalDevice(), m_WindowSurface, &presentModeCount, presentmodes.data());
		for (auto& mode : presentmodes)
		{
			if (mode == VK_PRESENT_MODE_MAILBOX_KHR)
			{
				presentMode = mode;
				break;
			}
		}

		// create the swap chain
		VkSwapchainCreateInfoKHR swapCreateInfo{};
		swapCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		swapCreateInfo.surface = m_WindowSurface;
		swapCreateInfo.minImageCount = bufferCount;
		swapCreateInfo.imageFormat = m_SurfaceFormat.format;
		swapCreateInfo.imageColorSpace = m_SurfaceFormat.colorSpace;
		swapCreateInfo.imageExtent = {width, height};
		swapCreateInfo.imageArrayLayers = 1;
		swapCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
		swapCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		swapCreateInfo.queueFamilyIndexCount = 0; // ignored if image sharing mode is exclusive
		swapCreateInfo.pQueueFamilyIndices = nullptr; // ignored if image sharing mode is exclusive
		swapCreateInfo.preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
		swapCreateInfo.compositeAlpha = VkCompositeAlphaFlagBitsKHR::VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		swapCreateInfo.presentMode = presentMode;
		swapCreateInfo.clipped = VK_TRUE;
		swapCreateInfo.oldSwapchain = m_SwapChain;

		VkResult result = vkCreateSwapchainKHR(api.GetDevice(), &swapCreateInfo, nullptr, &m_SwapChain);

		CORE_ASSERT(result == VK_SUCCESS, 
			"Failed to create swap chain");

		// get swap chain images
		uint32 imageCount = 0;
		vkGetSwapchainImagesKHR(api.GetDevice(), m_SwapChain, &imageCount, nullptr);
		DestroyImages();
		m_Images.resize(imageCount);
		vkGetSwapchainImagesKHR(api.GetDevice(), m_SwapChain, &imageCount, m_Images.data());
		
		// create image views
		m_ImageViews.resize(imageCount);
		for (uint32 i = 0; i < imageCount; i++)
		{
			VkImageViewCreateInfo imageViewCreateInfo{};
			imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			imageViewCreateInfo.image = m_Images[i];
			imageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
			imageViewCreateInfo.format = m_SurfaceFormat.format;
			imageViewCreateInfo.components = {	VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY, 
												VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY };
			imageViewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			imageViewCreateInfo.subresourceRange.baseMipLevel = 0;
			imageViewCreateInfo.subresourceRange.levelCount = 1;
			imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
			imageViewCreateInfo.subresourceRange.layerCount = 1;
			CORE_ASSERT(vkCreateImageView(api.GetDevice(), &imageViewCreateInfo, nullptr, &m_ImageViews[i]) == VK_SUCCESS,
				"failed to create image view for swapchain");
		}



		

	}

	VulkanSwapChain::~VulkanSwapChain()
	{
		VulkanRendererAPI& api = *(VulkanRendererAPI*)RendererCommand::GetApiInstance();
		DestroyImages();
		vkDestroySwapchainKHR(api.GetDevice(), m_SwapChain, nullptr);
		vkDestroySurfaceKHR(api.GetInstance(), m_WindowSurface, nullptr);
	}

	void VulkanSwapChain::Resize(uint32 width, uint32 height)
	{

	}

	void VulkanSwapChain::DestroyImages()
	{
		VulkanRendererAPI& api = *(VulkanRendererAPI*)RendererCommand::GetApiInstance();

		for (auto i : m_Images)
			vkDestroyImage(api.GetDevice(), i, nullptr);
		m_Images.clear();

		for (auto i : m_ImageViews)
			vkDestroyImageView(api.GetDevice(), i, nullptr);
		m_ImageViews.clear();
	}

}