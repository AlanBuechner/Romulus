#include "pch.h"
#include "VulkanSwapChain.h"
#include "Engine\Renderer\RendererCommand.h"

#include "VulkanRendererAPI.h"
#include "VulkanFrameBuffer.h"

#if defined(PLATFORM_WINDOWS)
#include "Platform\Windows\WindowsWindow.h"
#endif

namespace Engine
{

	VulkanSwapChain::VulkanSwapChain(uint32 width, uint32 height, void* window)
	{
		m_Window = window;
		GenSwapChain(width, height);
		GetNextImage();
	}

	VulkanSwapChain::~VulkanSwapChain()
	{
		DestroySwapChain();
	}

	void VulkanSwapChain::Update()
	{
		if (m_Regenerate)
		{
			DestroySwapChain();
			GenSwapChain(m_NewWidth, m_NewHeight);
		}

		GetNextImage();
	}

	Ref<FrameBuffer> VulkanSwapChain::GetBackBuffer()
	{
		return m_FrameBuffers[m_FrontIndex];
	}

	void VulkanSwapChain::Swap()
	{

		VulkanRendererAPI& api = *(VulkanRendererAPI*)RendererCommand::GetApiInstance();

		// present the image
		VkResult presentResult = VkResult::VK_RESULT_MAX_ENUM;
		VkPresentInfoKHR presentInfo{};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		presentInfo.waitSemaphoreCount = 0;
		presentInfo.pWaitSemaphores = nullptr;
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = &m_SwapChain;
		presentInfo.pImageIndices = &m_FrontIndex;
		presentInfo.pResults = &presentResult;
		VkResult result = vkQueuePresentKHR(api.GetQueue(), &presentInfo);
		CORE_ASSERT(result == VK_SUCCESS || result == VK_SUBOPTIMAL_KHR,
			"failed to present image");
		if (result == VK_SUBOPTIMAL_KHR)
			CORE_WARN("swapchain no longer matches window surface but can still be presented");
		CORE_ASSERT(presentResult == VK_SUCCESS || presentResult == VK_SUBOPTIMAL_KHR, "failed to present image");
		
	}

	void VulkanSwapChain::GenSwapChain(uint32 width, uint32 height)
	{
		m_Width = width;
		m_Height = height;

		VulkanRendererAPI& api = *(VulkanRendererAPI*)RendererCommand::GetApiInstance();

		// create the window surface
#if defined(PLATFORM_WINDOWS)
		WindowsWindow& win = *(WindowsWindow*)m_Window;

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

		std::vector< VkSurfaceFormatKHR> surfaceFormats{ formatCount };
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
		swapCreateInfo.imageExtent = { width, height };
		swapCreateInfo.imageArrayLayers = 1;
		swapCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
		swapCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		swapCreateInfo.queueFamilyIndexCount = 0; // ignored if image sharing mode is exclusive
		swapCreateInfo.pQueueFamilyIndices = nullptr; // ignored if image sharing mode is exclusive
		swapCreateInfo.preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
		swapCreateInfo.compositeAlpha = VkCompositeAlphaFlagBitsKHR::VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		swapCreateInfo.presentMode = presentMode;
		swapCreateInfo.clipped = VK_TRUE;
		swapCreateInfo.oldSwapchain = VK_NULL_HANDLE;

		VkResult result = vkCreateSwapchainKHR(api.GetDevice(), &swapCreateInfo, nullptr, &m_SwapChain);

		CORE_ASSERT(result == VK_SUCCESS,
			"Failed to create swap chain");

		// get swap chain images
		uint32 imageCount = 0;
		vkGetSwapchainImagesKHR(api.GetDevice(), m_SwapChain, &imageCount, nullptr);
		std::vector<VkImage> images{ imageCount };
		vkGetSwapchainImagesKHR(api.GetDevice(), m_SwapChain, &imageCount, images.data());

		// create image views
		std::vector<VkImageView> imageViews{ imageCount };
		for (uint32 i = 0; i < imageCount; i++)
		{
			VkImageViewCreateInfo imageViewCreateInfo{};
			imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			imageViewCreateInfo.image = images[i];
			imageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
			imageViewCreateInfo.format = m_SurfaceFormat.format;
			imageViewCreateInfo.components = { VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY,
												VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY };
			imageViewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			imageViewCreateInfo.subresourceRange.baseMipLevel = 0;
			imageViewCreateInfo.subresourceRange.levelCount = 1;
			imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
			imageViewCreateInfo.subresourceRange.layerCount = 1;
			CORE_ASSERT(vkCreateImageView(api.GetDevice(), &imageViewCreateInfo, nullptr, &imageViews[i]) == VK_SUCCESS,
				"failed to create image view for swapchain");
		}

		// create vulkan textures
		m_Images.clear();
		m_Images.resize(imageCount);
		for (uint32 i = 0; i < imageCount; i++)
		{
			m_Images[i] = std::make_shared<VulkanTexture>();
			((VulkanTexture*)m_Images[i].get())->CreateImage(width, height, images[i], imageViews[i],
				m_SurfaceFormat.format, VK_NULL_HANDLE, VK_IMAGE_LAYOUT_UNDEFINED, TextureFormat::Screen);
		}


		m_DepthStencilImage = Texture::Create(width, height, TextureFormat::DepthStencil, nullptr);

		// create frame buffers
		m_FrameBuffers.resize(imageCount);
		for (uint32 i = 0; i < imageCount; i++)
		{
			std::vector<FrameBufferAttachments> attachments{ 2 };
			attachments[0].format = TextureFormat::Screen;
			attachments[0].texture = m_Images[i];

			attachments[1].format = TextureFormat::DepthStencil;
			attachments[1].texture = m_DepthStencilImage;

			m_FrameBuffers[i] = FrameBuffer::Create(width, height, attachments);
		}

		CORE_INFO("regenerating window");

		m_Regenerate = false;
	}

	void VulkanSwapChain::DestroySwapChain()
	{
		VulkanRendererAPI& api = *(VulkanRendererAPI*)RendererCommand::GetApiInstance();
		if (m_AquireFence != VK_NULL_HANDLE)
		{
			vkDestroyFence(api.GetDevice(), m_AquireFence, nullptr);
			m_AquireFence = VK_NULL_HANDLE;
		}

		m_Images.clear();
		vkDestroySwapchainKHR(api.GetDevice(), m_SwapChain, nullptr);
		vkDestroySurfaceKHR(api.GetInstance(), m_WindowSurface, nullptr);
	}

	void VulkanSwapChain::GetNextImage()
	{
		VulkanRendererAPI& api = *(VulkanRendererAPI*)RendererCommand::GetApiInstance();

		if (m_AquireFence == VK_NULL_HANDLE)
		{
			VkFenceCreateInfo info{};
			info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
			vkCreateFence(api.GetDevice(), &info, nullptr, &m_AquireFence);
		}

		// get the next image 
		VkResult result = vkAcquireNextImageKHR(api.GetDevice(), m_SwapChain, UINT64_MAX, 0, m_AquireFence, &m_FrontIndex);
		CORE_ASSERT(result == VK_SUCCESS || result == VK_SUBOPTIMAL_KHR, "failed to get next image");
		if (result == VK_SUBOPTIMAL_KHR)
			CORE_WARN("swapchain no loger matches window surface but can still be acquired");

		// wait for v-sync
		vkWaitForFences(api.GetDevice(), 1, &m_AquireFence, VK_TRUE, UINT64_MAX);
		vkResetFences(api.GetDevice(), 1, &m_AquireFence);
		vkQueueWaitIdle(api.GetQueue());

	}

}