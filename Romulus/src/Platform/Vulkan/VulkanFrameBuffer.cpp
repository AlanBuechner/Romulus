#include "pch.h"
#include "VulkanFrameBuffer.h"

#include "Engine\Renderer\RendererCommand.h"
#include "VulkanRendererAPI.h"

#include "Engine\Renderer\RenderPass.h"
#include "VulkanRenderPass.h"

#include "VulkanTexture.h"

namespace Engine
{

	VulkanFrameBuffer::VulkanFrameBuffer(uint32 width, uint32 height, std::vector<FrameBufferAttachments> attachments)
	{
		m_Attachments = attachments;
		m_Width = width;
		m_Height = height;

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
		VulkanRendererAPI& api = *(VulkanRendererAPI*)RendererCommand::GetApiInstance();


		// create the vulkan frame buffer
		std::vector<VkImageView> attachments{m_Attachments.size()};
		for (uint32 i = 0; i < m_Attachments.size(); i++)
			attachments[i] = ((VulkanTexture*)m_Attachments[i].texture.get())->GetView();
		
		if (m_FrameBuffer != VK_NULL_HANDLE)
			DestroyFrameBuffer();

		VkFramebufferCreateInfo fbCreateInfo{};
		fbCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		fbCreateInfo.renderPass = ((VulkanRenderPass&)renderPass).GetRenderPass();
		fbCreateInfo.attachmentCount = (uint32)attachments.size();
		fbCreateInfo.pAttachments = attachments.data();
		fbCreateInfo.width = m_Width;
		fbCreateInfo.height = m_Height;
		fbCreateInfo.layers = 1;

		CORE_ASSERT(vkCreateFramebuffer(api.GetDevice(), &fbCreateInfo, nullptr, &m_FrameBuffer) == VK_SUCCESS,
			"failed to bind frame buffer to render pass");
		
	}

	void VulkanFrameBuffer::DestroyFrameBuffer()
	{
		VulkanRendererAPI& api = *(VulkanRendererAPI*)RendererCommand::GetApiInstance();

		vkDestroyFramebuffer(api.GetDevice(), m_FrameBuffer, nullptr);
		m_FrameBuffer = VK_NULL_HANDLE;
	}

}