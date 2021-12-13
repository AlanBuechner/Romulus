#pragma once
#include "Engine\Core\Core.h"
#include "Engine\Renderer\Shader.h"

#include <vulkan\vulkan.h>

namespace Engine
{
	class VulkanShader : public Shader
	{
	public:
		VulkanShader(Ref<ShaderSorce> src);
		virtual ~VulkanShader();

		virtual void Bind() const override;
		virtual void Unbind() const override;

	private:
		std::vector<uint32> CompileShaderToSpirv(ShaderType type);
		VkShaderModule CreateShaderModual(std::vector<uint32> data);

		Ref<ShaderSorce> m_ShaderPaths;
		std::unordered_map<ShaderType, VkShaderModule> m_ShaderModuals;
	};
}
