#include "pch.h"
#include "VulkanCommands.h"
#include "Engine\Core\Application.h"
#include "VulkanRenderPass.h"
#include "VulkanFrameBuffer.h"
#include "VulkanTexture.h"

namespace Engine
{

	void CreateBeginRenderpassCommand(const BeginRenderPassCommand& cmd, VkCommandBuffer buffer, const RenderPass& renderPass)
	{
		// bind the frame buffer to the render pass
		Ref<FrameBuffer> destBuffer = cmd.buffer;
		if (!destBuffer) // if not desitnation buffer is specified draw the the swap chins front buffer
			destBuffer = Application::GetWindow()->GetSwapChain().GetBackBuffer();
		destBuffer->BindToRenderPass(renderPass);

		std::array<VkClearValue, 2> clearColor{};
		clearColor[0].color.float32[0] = cmd.color.r;
		clearColor[0].color.float32[1] = cmd.color.g;
		clearColor[0].color.float32[2] = cmd.color.b;
		clearColor[0].color.float32[3] = cmd.color.a;
		clearColor[1].depthStencil = { 0.0f , 0 };

		VkRenderPassBeginInfo beginRenderPassInfo{};
		beginRenderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		beginRenderPassInfo.renderPass = ((const VulkanRenderPass&)renderPass).GetRenderPass();
		beginRenderPassInfo.framebuffer = (*(VulkanFrameBuffer*)destBuffer.get()).GetVulkanFrameBuffer();
		beginRenderPassInfo.renderArea = { {0, 0}, {destBuffer->GetWidth(), destBuffer->GetHeight()} };
		beginRenderPassInfo.clearValueCount = (uint32)clearColor.size();
		beginRenderPassInfo.pClearValues = clearColor.data();

		vkCmdBeginRenderPass(buffer, &beginRenderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

		VkViewport viewport{0,0, destBuffer->GetWidth(), destBuffer->GetHeight(), 0, 1};
		vkCmdSetViewport(buffer, 0, 1, &viewport);

		vkCmdSetScissor(buffer, 0, 1, &beginRenderPassInfo.renderArea);


	}

	void CreateEndRenderpassCommand(VkCommandBuffer buffer)
	{
		vkCmdEndRenderPass(buffer);
	}

	void CreateRenderCommand(const CommandBuffer& cmd, VkCommandBuffer buffer, const RenderPass& renderPass)
	{
		switch (cmd.type)
		{
		case CommandType::BeginRenderPassCommand:
			return CreateBeginRenderpassCommand((const BeginRenderPassCommand&)cmd, buffer, renderPass);
		case CommandType::EndRenderPassCommand:
			return CreateEndRenderpassCommand(buffer);
		default:
			break;
		}
	}

}