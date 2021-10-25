#include "pch.h"
#include "VulkanFrameBuffer.h"

#include "Engine\Renderer\RenderPass.h"
#include "VulkanRenderPass.h"

namespace Engine
{

	VulkanFrameBuffer::VulkanFrameBuffer(uint32 width, uint32 height, std::vector<FrameBufferAttachments> attachments)
	{
		m_Attachments = attachments;

		// create attachments
		for (auto attachment : m_Attachments)
			if (!attachment.texture)
				attachment.texture = Texture::Create(width, height, attachment.format);

	}

	VulkanFrameBuffer::~VulkanFrameBuffer()
	{
	}

	void VulkanFrameBuffer::BindToRenderPass(RenderPass& renderPass)
	{
		// create the vulkan frame buffer

		
	}

}