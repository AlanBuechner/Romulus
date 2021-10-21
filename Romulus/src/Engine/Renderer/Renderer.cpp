#include "pch.h"
#include "Renderer.h"

namespace Engine
{
	void Renderer::Init(API api)
	{
		RendererCommand::Init(api);
	}

	void Renderer::Destroy()
	{
		RendererCommand::Destroy();
	}

}