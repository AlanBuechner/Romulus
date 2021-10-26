#pragma once
#include "Engine\Core\Core.h"
#include "Engine\Renderer\SwapChain.h"
#include "Engine\Renderer\FrameBuffer.h"

#include "VulkanTexture.h"

#include <vulkan\vulkan.h>

namespace Engine
{
	class VulkanSwapChain : public SwapChain
	{
	public:
		VulkanSwapChain() = delete;
		VulkanSwapChain(uint32 width, uint32 height, void* window);
		~VulkanSwapChain();

		virtual void Resize(uint32 width, uint32 height) override;
		virtual Ref<FrameBuffer> GetBackBuffer() override;

		virtual void Swap() override;
		
	private:
		VkSurfaceKHR m_WindowSurface = VK_NULL_HANDLE;
		VkSurfaceFormatKHR m_SurfaceFormat = {};

		VkSwapchainKHR m_SwapChain = VK_NULL_HANDLE;

		std::vector<Ref<Texture>> m_Images;
		Ref<Texture> m_DepthStencilImage;
		std::vector<Ref<FrameBuffer>> m_FrameBuffers;

		uint32 m_FrontIndex = 0;
		VkFence m_AquireFence = VK_NULL_HANDLE;
	};
}

