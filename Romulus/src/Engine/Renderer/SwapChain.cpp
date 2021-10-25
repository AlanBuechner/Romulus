#include "pch.h"
#include "SwapChain.h"
#include "RendererCommand.h"

#include "Platform\Vulkan\VulkanSwapChain.h"

namespace Engine
{
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