#include "pch.h"
#include "VulkanRenderPass.h"
#include "Engine\Core\Application.h"
#include "Engine\Renderer\RendererCommand.h"

#include "VulkanRendererAPI.h"
#include "VulkanSwapChain.h"
#include "VulkanFrameBuffer.h"


namespace Engine
{
	VulkanRenderPass::~VulkanRenderPass()
	{
		VulkanRendererAPI& api = *(VulkanRendererAPI*)RendererCommand::GetApiInstance();
		vkDestroyRenderPass(api.GetDevice(), m_RenderPass, nullptr);
	}

	void VulkanRenderPass::GenerateRenderPass()
	{
		VulkanRendererAPI& api = *(VulkanRendererAPI*)RendererCommand::GetApiInstance();
		
		// get the destination frame buffer
		bool destIsSwap = false;
		Ref<FrameBuffer> destBuffer = m_DestinationBuffer;
		if (!destBuffer) // if not desitnation buffer is specified draw the the swap chins front buffer
		{
			destBuffer = Application::GetWindow()->GetSwapChain().GetFrontBuffer();
			destIsSwap = true;
		}

		// get the attachments for the destination frame buffer
		std::vector<FrameBufferAttachments>& frameAttachments = destBuffer->GetAttachments();

		// find the load operation for the destination buffer
		VkAttachmentLoadOp loadop = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		switch (m_ClearType)
		{
		case Engine::RenderPass::ClearType::None:
			loadop = VK_ATTACHMENT_LOAD_OP_LOAD;
			break;
		case Engine::RenderPass::ClearType::Color:
			loadop = VK_ATTACHMENT_LOAD_OP_CLEAR;
			break;
		default:
			break;
		}

		// create vulkan attatchment desctiptions 
		std::vector<VkAttachmentDescription> attachments{ frameAttachments.size() };
		for (uint32 i = 0; i < frameAttachments.size(); i++)
		{
			FrameBufferAttachments& attachment = frameAttachments[i];
			VulkanTexture& image = *(VulkanTexture*)attachment.texture.get();

			VkImageLayout finalLayout = VK_IMAGE_LAYOUT_UNDEFINED;
			switch (image.GetFormat())
			{
			case TextureFormat::DepthStencil:
				finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
				break;
			case TextureFormat::Depth:
				finalLayout = VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL;
				break;
			case TextureFormat::Stencil:
				finalLayout = VK_IMAGE_LAYOUT_STENCIL_ATTACHMENT_OPTIMAL;
				break;
			case TextureFormat::Screen:
				finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
				break;
			default:
				break;
			}

			attachments[i].flags = 0;
			attachments[i].format = image.GetVulkanFormat();
			attachments[i].samples = VK_SAMPLE_COUNT_1_BIT;
			attachments[i].loadOp = loadop;
			attachments[i].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
			attachments[i].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_LOAD;
			attachments[i].stencilStoreOp = VK_ATTACHMENT_STORE_OP_STORE;
			attachments[i].initialLayout = image.GetVulkanLayout();
			attachments[i].finalLayout = finalLayout;

			image.SetVulkanLayout(finalLayout);
		}

		std::vector<VkSubpassDescription> subPasses;
		
		// TODO : generate sub passes
		std::array<VkAttachmentReference, 1> colorAttachments;
		colorAttachments[0].layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
		colorAttachments[0].attachment = 0;

		VkAttachmentReference depthStencilAttachment;
		depthStencilAttachment.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
		depthStencilAttachment.attachment = 1;

		subPasses.resize(1);
		subPasses[0].pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		subPasses[0].inputAttachmentCount = 0;
		subPasses[0].pInputAttachments = nullptr;
		subPasses[0].colorAttachmentCount = (uint32)colorAttachments.size();
		subPasses[0].pColorAttachments = colorAttachments.data();
		subPasses[0].pDepthStencilAttachment = &depthStencilAttachment;

		// create the render pass
		VkRenderPassCreateInfo rpCreateInfo{};
		rpCreateInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		rpCreateInfo.attachmentCount = (uint32)attachments.size();
		rpCreateInfo.pAttachments = attachments.data();
		rpCreateInfo.subpassCount = (uint32)subPasses.size();
		rpCreateInfo.pSubpasses = subPasses.data();
		rpCreateInfo.dependencyCount = 0;
		rpCreateInfo.pDependencies = nullptr;

		CORE_ASSERT(vkCreateRenderPass(api.GetDevice(), &rpCreateInfo, nullptr, &m_RenderPass) == VK_SUCCESS,
			"failed to create render pass");

		// bind the frame buffer to the render pass
		destBuffer->BindToRenderPass(*this);
	}


}