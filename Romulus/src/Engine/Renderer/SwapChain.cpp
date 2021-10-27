#include "pch.h"
#include "SwapChain.h"
#include "RendererCommand.h"

#include "Platform\Vulkan\VulkanSwapChain.h"

namespace Engine
{
	void SwapChain::Resize(uint32 width, uint32 height)
	{
		m_Regenerate = true;
		m_NewWidth = width;
		m_NewHeight = height;
	}

	Scope<SwapChain> SwapChain::Create(uint32 width, uint32 height, void* window)
	{
		switch (RendererCommand::GetApi())
		{
		case API::Vulkan:
			return CreateScope<VulkanSwapChain>(width, height, window);
		default:
			break;
		}

		return Scope<SwapChain>();
	}

}