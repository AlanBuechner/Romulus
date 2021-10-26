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
		Ref<FrameBuffer> destBuffer = m_DestinationBuffer;
		if (!destBuffer) // if not desitnation buffer is specified draw the the swap chins front buffer
			destBuffer = Application::GetWindow()->GetSwapChain().GetBackBuffer();
		

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


		// create command pool
		VkCommandPoolCreateInfo poolCreateInfo{};
		poolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		poolCreateInfo.flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT | VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
		poolCreateInfo.queueFamilyIndex = api.GetDeviceQueueFamilyIndices(api.GetPhysicalDevice()).graphicsFamily.value();

		CORE_ASSERT(vkCreateCommandPool(api.GetDevice(), &poolCreateInfo, nullptr, &m_Pool) == VK_SUCCESS,
			"failed to create command pool");

		// create command buffer
		VkCommandBufferAllocateInfo allocateInfo{};
		allocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocateInfo.commandPool = m_Pool;
		allocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocateInfo.commandBufferCount = 1;

		CORE_ASSERT(vkAllocateCommandBuffers(api.GetDevice(), &allocateInfo, &m_ClearBuffer) == VK_SUCCESS,
			"failed to allocate memory for command buffers");

	}

	void VulkanRenderPass::Run()
	{
		VulkanRendererAPI& api = *(VulkanRendererAPI*)RendererCommand::GetApiInstance();

		// bind the frame buffer to the render pass
		Ref<FrameBuffer> destBuffer = m_DestinationBuffer;
		if (!destBuffer) // if not desitnation buffer is specified draw the the swap chins front buffer
			destBuffer = Application::GetWindow()->GetSwapChain().GetBackBuffer();
		destBuffer->BindToRenderPass(*this);

		VkCommandBufferBeginInfo clearInfo{};
		clearInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		clearInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
		CORE_ASSERT(vkBeginCommandBuffer(m_ClearBuffer, &clearInfo) == VK_SUCCESS,
			"failed to begin command buffer");

		std::array<VkClearValue, 2> clearColor{};
		clearColor[0].color.float32[0] = m_ClearColor.r;
		clearColor[0].color.float32[1] = m_ClearColor.g;
		clearColor[0].color.float32[2] = m_ClearColor.b;
		clearColor[0].color.float32[3] = m_ClearColor.a;
		clearColor[1].depthStencil = { 0.0f , 0 };

		VkRenderPassBeginInfo beginRenderPassInfo{};
		beginRenderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		beginRenderPassInfo.renderPass = m_RenderPass;
		beginRenderPassInfo.framebuffer = (*(VulkanFrameBuffer*)destBuffer.get()).GetVulkanFrameBuffer();
		beginRenderPassInfo.renderArea = { {0, 0}, {destBuffer->GetWidth(), destBuffer->GetHeight()} };
		beginRenderPassInfo.clearValueCount = (uint32)clearColor.size();
		beginRenderPassInfo.pClearValues = clearColor.data();

		vkCmdBeginRenderPass(m_ClearBuffer, &beginRenderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

		vkCmdEndRenderPass(m_ClearBuffer);

		vkEndCommandBuffer(m_ClearBuffer);

		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.waitSemaphoreCount = 0;
		submitInfo.pWaitSemaphores = nullptr;
		submitInfo.pWaitDstStageMask = nullptr;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &m_ClearBuffer;
		submitInfo.signalSemaphoreCount = 0;
		submitInfo.pSignalSemaphores = nullptr;

		vkQueueSubmit(api.GetQueue(), 1, &submitInfo, VK_NULL_HANDLE);

	}


}