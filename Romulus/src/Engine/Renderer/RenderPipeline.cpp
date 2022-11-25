#include "pch.h"
#include "RenderPipeline.h"
#include "RendererCommand.h"

#include "Platform\Vulkan\VulkanRenderPipeline.h"

namespace Engine
{

	Ref<RenderPipeline> RenderPipeline::Create(Ref<Shader> shader, Ref<RenderPass> renderPass, Topology topology, bool wireframe)
	{
		switch (RendererCommand::GetApi())
		{
		case API::Vulkan:
			return CreateRef<VulkanRenderPipeline>(shader, renderPass, topology, wireframe);
		default:
			break;
		}
		return Ref<RenderPipeline>();
	}

}