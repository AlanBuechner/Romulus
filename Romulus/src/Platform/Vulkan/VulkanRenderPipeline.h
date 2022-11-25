#pragma once
#include "Engine\Core\Core.h"
#include "Engine\Renderer\RenderPipeline.h"
#include "Engine\Renderer\Shader.h"

#include <vulkan\vulkan.h>

namespace Engine
{
	class VulkanRenderPipeline : public RenderPipeline
	{
	public:
		VulkanRenderPipeline(Ref<Shader> shader, Ref<RenderPass> renderPass, Topology topology, bool wireframe);
		~VulkanRenderPipeline();

	private:

		VkPipeline m_Pipeline = VK_NULL_HANDLE;
		VkPipelineLayout m_PipelineLayout = VK_NULL_HANDLE;

	};
}