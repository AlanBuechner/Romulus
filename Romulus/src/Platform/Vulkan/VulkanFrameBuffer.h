#pragma once
#include "Engine\Core\Core.h"
#include "Engine\Renderer\FrameBuffer.h"

#include <vulkan\vulkan.h>

namespace Engine
{
	class VulkanFrameBuffer : public FrameBuffer
	{
	public:
		VulkanFrameBuffer() = delete;
		VulkanFrameBuffer(uint32 width, uint32 height, std::vector<FrameBufferAttachments> attachments);
		~VulkanFrameBuffer();

		virtual void BindToRenderPass(RenderPass& renderPass) override;

		VkFramebuffer GetVulkanFrameBuffer() { return m_FrameBuffer; }

	private:
		void DestroyFrameBuffer();

	private:
		VkFramebuffer m_FrameBuffer = VK_NULL_HANDLE;
		
	};
}