#include "pch.h"
#include "RendererCommand.h"

#include "RendererAPI.h"

#include "Platform\Vulkan\VulkanRendererAPI.h"

Engine::API Engine::RendererCommand::s_Api;
Engine::RendererAPI* Engine::RendererCommand::s_ApiInstance = nullptr;

namespace Engine
{
	void RendererCommand::Init(API api)
	{
		s_Api = api;

		switch (s_Api)
		{
		case Engine::API::Vulkan:
			CORE_INFO("Initializing Vulkan!");
			s_ApiInstance = new VulkanRendererAPI();
			break;
		default:
			CORE_ASSERT(false, "API NOT SELECTED OR NOT SUPPORTED");
			break;
		}
	}

	void RendererCommand::Destroy()
	{
		delete s_ApiInstance;
	}
}
