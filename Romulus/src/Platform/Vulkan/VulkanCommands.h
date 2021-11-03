#pragma once
#include "Engine\Renderer\CommandBuffers.h"

#include <vulkan\vulkan.h>

namespace Engine
{
	void CreateBeginRenderpassCommand(const BeginRenderPassCommand& cmd, VkCommandBuffer buffer, const RenderPass& renderPass);
	void CreateEndRenderpassCommand(VkCommandBuffer buffer);

	void CreateRenderCommand(const CommandBuffer& cmd, VkCommandBuffer buffer, const RenderPass& renderPass);

}