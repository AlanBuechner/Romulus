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

	private:
		VkRenderPass m_RenderPass = VK_NULL_HANDLE;

	};
}
