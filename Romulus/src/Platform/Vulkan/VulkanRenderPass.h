#pragma once
#include "Engine\Core\Core.h"
#include "Engine\Renderer\RenderPass.h"

#include <vulkan\vulkan.h>

namespace Engine
{
	class VulkanRenderPass : public RenderPass
	{
	public:
		~VulkanRenderPass();

		virtual void GenerateRenderPass() override;
		virtual void Run() override;

		inline VkRenderPass GetRenderPass() { return m_RenderPass; }

	private:
		VkRenderPass m_RenderPass = VK_NULL_HANDLE;

		VkCommandPool m_Pool;
		VkCommandBuffer m_ClearBuffer = VK_NULL_HANDLE;

	};
}
