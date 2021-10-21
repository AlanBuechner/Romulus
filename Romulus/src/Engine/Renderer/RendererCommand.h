#pragma once
#include "Engine\Core\Core.h"

namespace Engine
{
	class RendererAPI;
}

namespace Engine
{
	enum class API
	{
		Vulkan
	};

	class RendererCommand
	{
	public:
		static void Init(API api);
		static void Destroy();
		static API GetApi() { return s_Api; }
		static RendererAPI* GetApiInstance() { return s_ApiInstance; }

	private:
		static API s_Api;
		static RendererAPI* s_ApiInstance;

	};
}
