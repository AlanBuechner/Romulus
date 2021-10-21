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
		VkSurfaceKHR m_WindowSurface = VK_NULL_HANDLE;
		VkSurfaceFormatKHR m_SurfaceFormat = {};

	};
}

