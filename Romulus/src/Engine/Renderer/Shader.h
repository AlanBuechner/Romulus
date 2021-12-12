#pragma once
#include "Engine\Core\Core.h"
#include <filesystem>

namespace Engine
{
	class Shader
	{
	public:
		enum ShaderType
		{
			None = -1, Vertex = BIT(0), Pixle = BIT(1), Geometry = BIT(2)
		};

		struct ShaderSorce
		{
		public:
			std::filesystem::path vertexPath;
			std::filesystem::path pixlePath;
			std::filesystem::path geometryPath;

			void operator<<(const ShaderSorce& other)
			{
				if (this->vertexPath.empty())
					this->vertexPath = other.vertexPath;
				if (this->pixlePath.empty())
					this->pixlePath = other.pixlePath;
				if (this->geometryPath.empty())
					this->geometryPath = other.geometryPath;
			}

			std::string GetShaderSrc(ShaderType type);
		};


	public:
		virtual ~Shader() {};

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		static ShaderSorce LoadShader(const std::filesystem::path& file);
		static ShaderSorce LoadShader(const std::filesystem::path& file, int type);
		static void ReloadShader(ShaderSorce& shaders);

		static Ref<Shader> Create(const std::filesystem::path& src);
		static Ref<Shader> Create(Ref<ShaderSorce> src);

	private:
	};
}