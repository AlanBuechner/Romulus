#include "pch.h"
#include "Shader.h"
#include "RendererCommand.h"

#include "Platform\Vulkan\VulkanShader.h"

#include <fstream>

namespace Engine
{

	std::string Shader::ShaderSorce::GetShaderSrc(ShaderType type)
	{
		std::filesystem::path file;
		switch (type)
		{
		case Engine::Shader::Vertex:
			file = vertexPath;
			break;
		case Engine::Shader::Pixle:
			file = pixlePath;
			break;
		case Engine::Shader::Geometry:
			file = geometryPath;
			break;
		case Engine::Shader::None:
		default:
			return "";

		}

		std::ifstream shaderFile;
		shaderFile.open(file);
		if (shaderFile.fail())
		{
			CORE_ERROR("Cant open file {0}", file);
			return "";
		}

		enum class Type
		{
			NONE = -1, VERTEX = 0, PIXLE = 1, GEOMETRY = 2
		};

		Type mode = Type::NONE;

		std::stringstream ss[3];

		std::string line;
		while (getline(shaderFile, line))
		{
			if (line.find("#shader") != std::string::npos)
			{
				if (line.find("vertex") != std::string::npos)
					mode = Type::VERTEX;
				else if (line.find("pixle") != std::string::npos || line.find("fragment") != std::string::npos)
					mode = Type::PIXLE;
				else if (line.find("geometry") != std::string::npos)
					mode = Type::GEOMETRY;
			}
			else
			{
				ss[(int)mode] << line << '\n';
			}
		}

		shaderFile.close();

		switch (type)
		{
		case Engine::Shader::Vertex:	return ss[(int)Type::VERTEX].str();
		case Engine::Shader::Pixle:		return ss[(int)Type::PIXLE].str();
		case Engine::Shader::Geometry:	return ss[(int)Type::GEOMETRY].str();
		case Engine::Shader::None:
		default:
			return "";
		}
	}

	Shader::ShaderSorce Shader::LoadShader(const std::filesystem::path& file, int type)
	{
		ShaderSorce sorce;
		if ((type & (int)ShaderType::Vertex) != 0)
			sorce.vertexPath = file;
		if ((type & (int)ShaderType::Pixle) != 0)
			sorce.pixlePath = file;
		if ((type & (int)ShaderType::Geometry) != 0)
			sorce.geometryPath = file;

		return sorce;
	}

	Shader::ShaderSorce Shader::LoadShader(const std::filesystem::path& file)
	{
		ShaderSorce sorce;
		std::ifstream shaderFile;
		shaderFile.open(file);
		if (shaderFile.fail())
		{
			CORE_ERROR("Cant open file {0}", file);
			return ShaderSorce();
		}

		std::string line;
		while (getline(shaderFile, line))
		{
			if (line.find("#shader") != std::string::npos)
			{
				if (line.find("vertex") != std::string::npos)
					sorce.vertexPath = file;
				else if (line.find("pixle") != std::string::npos || line.find("fragment") != std::string::npos)
					sorce.pixlePath = file;
				else if (line.find("geometry") != std::string::npos)
					sorce.geometryPath = file;
			}
		}
		
		return sorce;
	}

	void Shader::ReloadShader(ShaderSorce& shaders)
	{
		// TODO
	}

	Ref<Shader> Engine::Shader::Create(const std::filesystem::path& file)
	{
		Ref<ShaderSorce> src = CreateRef<ShaderSorce>(LoadShader(file));
		return Create(src);
	}

	Ref<Shader> Shader::Create(Ref<ShaderSorce> src)
	{
		switch (RendererCommand::GetApi())
		{
		case API::Vulkan:
			return CreateRef<VulkanShader>(src);
		default:
			break;
		}


		return Ref<Shader>();
	}

}