#include "pch.h"
#include "RenderPass.h"
#include "RendererCommand.h"

#include "Platform\Vulkan\VulkanRenderPass.h"

namespace Engine
{
	Ref<RenderPass> Engine::RenderPass::Create()
	{
		switch (RendererCommand::GetApi())
		{
		case API::Vulkan:
			return CreateRef<VulkanRenderPass>();
		default:
			break;
		}

		return Ref<RenderPass>();
	}

}