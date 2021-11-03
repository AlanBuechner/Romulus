#include "pch.h"
#include "VulkanRenderPass.h"
#include "Engine\Core\Application.h"
#include "Engine\Renderer\RendererCommand.h"

#include "VulkanRendererAPI.h"
#include "VulkanSwapChain.h"
#include "VulkanFrameBuffer.h"

#include "Engine\Renderer\CommandBuffers.h"
#include "VulkanCommands.h"


namespace Engine
{
	VulkanRenderPass::~VulkanRenderPass()
	{
		VulkanRendererAPI& api = *(VulkanRendererAPI*)RendererCommand::GetApiInstance();
		vkDestroyRenderPass(api.GetDevice(), m_RenderPass, nullptr);
		vkDestroyCommandPool(api.GetDevice(), m_Pool, nullptr);
	}

	void VulkanRenderPass::GenerateRenderPass()
	{
		BeginRenderPassCommand beginCommand;
		beginCommand.color = m_ClearColor;
		beginCommand.buffer = m_DestinationBuffer;
		m_Commands.insert(m_Commands.begin(), CreateRef<BeginRenderPassCommand>(beginCommand));
		m_Commands.push_back(CreateRef<EndRenderPassCommand>());


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

		std::vector<VkSubpassDescription> subPasses{ 1 };

		static std::vector<VkAttachmentReference> s_AttachmentRefrences;

		// TODO : generate sub passes
		s_AttachmentRefrences.push_back({ 0, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL });
		s_AttachmentRefrences.push_back({ 1, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL });

		subPasses[0].pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		subPasses[0].inputAttachmentCount = 0;
		subPasses[0].pInputAttachments = nullptr;
		subPasses[0].colorAttachmentCount = 1;
		subPasses[0].pColorAttachments = &s_AttachmentRefrences[0];
		subPasses[0].pDepthStencilAttachment = &s_AttachmentRefrences[1];

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

		CORE_ASSERT(vkAllocateCommandBuffers(api.GetDevice(), &allocateInfo, &m_CommandBuffer) == VK_SUCCESS,
			"failed to allocate memory for command buffers");

	}

	void VulkanRenderPass::Execute()
	{
		VulkanRendererAPI& api = *(VulkanRendererAPI*)RendererCommand::GetApiInstance();

		// create command buffer
		VkCommandBufferBeginInfo clearInfo{};
		clearInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		CORE_ASSERT(vkBeginCommandBuffer(m_CommandBuffer, &clearInfo) == VK_SUCCESS,
			"failed to begin command buffer");

		for (uint32 i = 0; i < m_Commands.size(); i++)
			CreateRenderCommand(*m_Commands[i], m_CommandBuffer, *this);

		vkEndCommandBuffer(m_CommandBuffer);

		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.waitSemaphoreCount = 0;
		submitInfo.pWaitSemaphores = nullptr;
		submitInfo.pWaitDstStageMask = nullptr;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &m_CommandBuffer;
		submitInfo.signalSemaphoreCount = 0;
		submitInfo.pSignalSemaphores = nullptr;

		vkQueueSubmit(api.GetQueue(), 1, &submitInfo, VK_NULL_HANDLE);

	}


}