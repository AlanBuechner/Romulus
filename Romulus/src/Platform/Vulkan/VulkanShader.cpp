#include "pch.h"
#include "VulkanShader.h"
#include "Engine\Renderer\RendererCommand.h"
#include "VulkanRendererAPI.h"

#include <shaderc/shaderc.hpp>
#include <spirv_cross/spirv_cross.hpp>
#include <spirv_cross/spirv_glsl.hpp>

#include <fstream>

namespace Engine
{
	namespace Utils
	{
		static std::filesystem::path GetShaderCasheDirectory()
		{
			switch (RendererCommand::GetApi())
			{
			case API::Vulkan:
				return "Cashe/Shaders/Vulkan";
			default:
				break;
			}

			CORE_ASSERT(false, "no valid rendering api");
			return "";
		}

		static shaderc_shader_kind GetShadercKind(Shader::ShaderType type)
		{
			switch (type)
			{
			case Engine::Shader::Vertex:	return shaderc_glsl_vertex_shader;
			case Engine::Shader::Pixle:		return shaderc_glsl_fragment_shader;
			case Engine::Shader::Geometry:	return shaderc_glsl_geometry_shader;
			default:
				break;
			}
			
			CORE_ASSERT(false, "not valid shader type");
			return (shaderc_shader_kind)0;
		}

		std::string GetCashedShaderExtention(Shader::ShaderType type)
		{
			switch (type)
			{
			case Engine::Shader::Vertex:	return ".vert";
			case Engine::Shader::Pixle:		return ".frag";
			case Engine::Shader::Geometry:	return ".geom";
			default:
				break;
			}

			CORE_ASSERT(false, "not valid shader type");
			return "";
		}

	}

	VulkanShader::VulkanShader(Ref<ShaderSorce> src) : 
		m_ShaderPaths(src)
	{
		// create the cashe directory
		std::filesystem::path casheDirectory = Utils::GetShaderCasheDirectory();
		if (!std::filesystem::exists(casheDirectory))
			std::filesystem::create_directories(casheDirectory);

		if(!src->vertexPath.empty())
			m_ShaderModuals[ShaderType::Vertex] = CreateShaderModual(CompileShaderToSpirv(ShaderType::Vertex));
		if (!src->pixlePath.empty())
			m_ShaderModuals[ShaderType::Pixle] = CreateShaderModual(CompileShaderToSpirv(ShaderType::Pixle));
		if (!src->geometryPath.empty())
			m_ShaderModuals[ShaderType::Geometry] = CreateShaderModual(CompileShaderToSpirv(ShaderType::Geometry));

	}

	VulkanShader::~VulkanShader()
	{
		VulkanRendererAPI& api = *(VulkanRendererAPI*)RendererCommand::GetApiInstance();

		if(!m_ShaderPaths->vertexPath.empty())
			vkDestroyShaderModule(api.GetDevice(), m_ShaderModuals[ShaderType::Vertex], nullptr);
		if(!m_ShaderPaths->pixlePath.empty())
			vkDestroyShaderModule(api.GetDevice(), m_ShaderModuals[ShaderType::Pixle], nullptr);
		if(!m_ShaderPaths->geometryPath.empty())
			vkDestroyShaderModule(api.GetDevice(), m_ShaderModuals[ShaderType::Geometry], nullptr);
	}

	void VulkanShader::Bind() const
	{
	}

	void VulkanShader::Unbind() const
	{
	}

	std::vector<uint32> VulkanShader::CompileShaderToSpirv(ShaderType type)
	{
		std::filesystem::path path;
		switch (type)
		{
		case Engine::Shader::Vertex:
			path = m_ShaderPaths->vertexPath;
			break;
		case Engine::Shader::Pixle:
			path = m_ShaderPaths->pixlePath;
			break;
		case Engine::Shader::Geometry:
			path = m_ShaderPaths->geometryPath;
			break;
		case Engine::Shader::None:
		default:
			return std::vector<uint32>();
		}

		shaderc::Compiler compiler;
		shaderc::CompileOptions options;
		options.SetTargetEnvironment(shaderc_target_env_vulkan, shaderc_env_version_vulkan_1_1);
		options.SetOptimizationLevel(shaderc_optimization_level_performance);

		std::vector<uint32> binaryData;

		std::filesystem::path casheDirectory = Utils::GetShaderCasheDirectory();
		std::filesystem::path cashedFile = casheDirectory / (path.filename().string() + Utils::GetCashedShaderExtention(type));
		std::ifstream in(cashedFile, std::ios::in | std::ios::binary);
		if (in.is_open() && false)
		{
			// load cashed data
			in.seekg(0, std::ios::end);
			auto size = in.tellg();
			in.seekg(0, std::ios::beg);

			binaryData.resize(size / sizeof(uint32));
			in.read((char*)binaryData.data(), size);
		}
		else
		{
			// compile shader
			std::string src = m_ShaderPaths->GetShaderSrc(type);
			shaderc::SpvCompilationResult module = compiler.CompileGlslToSpv(src, Utils::GetShadercKind(type), path.string().c_str(), options);
			if (module.GetCompilationStatus() != shaderc_compilation_status_success)
			{
				CORE_ERROR(module.GetErrorMessage());
				CORE_ASSERT(false, "");
			}

			binaryData = std::vector<uint32>(module.cbegin(), module.cend());
			
			// cashe shader
			std::ofstream out(cashedFile, std::ios::out | std::ios::binary);
			if (out.is_open())
			{
				out.write((char*)binaryData.data(), binaryData.size() * sizeof(uint32));
				out.flush();
				out.close();
			}

		}

		return binaryData;
	}

	VkShaderModule VulkanShader::CreateShaderModual(std::vector<uint32> data)
	{
		VulkanRendererAPI& api = *(VulkanRendererAPI*)RendererCommand::GetApiInstance();

		VkShaderModuleCreateInfo smInfo{};
		smInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		smInfo.codeSize = data.size()*sizeof(uint32);
		smInfo.pCode = reinterpret_cast<uint32*>(data.data());

		VkShaderModule module;
		if (vkCreateShaderModule(api.GetDevice(), &smInfo, nullptr, &module) != VK_SUCCESS)
		{
			CORE_ERROR("Faild to create shader module");
		}

		return module;
	}

}