#pragma once
#include "Engine\Core\Core.h"
#include "Engine\Renderer\SwapChain.h"

#define VK_PROTOTYPES
#include <vulkan\vulkan.h>

namespace Engine
{
	class VulkanSwapChain : public SwapChain
	{
	public:
		VulkanSwapChain(uint32 width, uint32 height, void* window);
		~VulkanSwapChain();

		virtual void Resize(uint32 width, uint32 height) override;

	private:
		void DestroyImages();
		
	private:
		VkSurfaceKHR m_WindowSurface = VK_NULL_HANDLE;
		VkSurfaceFormatKHR m_SurfaceFormat = {};

		VkSwapchainKHR m_SwapChain = VK_NULL_HANDLE;

		std::vector<VkImage> m_Images;
		std::vector<VkImageView> m_ImageViews;

		VkImage m_DepthStencilBuffer = VK_NULL_HANDLE;
		VkImageView m_DepthStencilView = VK_NULL_HANDLE;
		VkFormat m_DepthSetncilFormat = VK_FORMAT_UNDEFINED;
		VkDeviceMemory m_DepthStencilMemory;

	};
}

