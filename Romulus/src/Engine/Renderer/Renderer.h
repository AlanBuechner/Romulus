#pragma once
#include "Engine\Core\Core.h"
#include "RendererCommand.h"

namespace Engine
{
	class Renderer
	{
	public:
		static void Init(API api);
		static void Destroy();
	};
}