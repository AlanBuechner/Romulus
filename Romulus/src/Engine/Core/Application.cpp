#include "pch.h"
#include "Application.h"
#include "Core.h"
#include "Time.h"
#include "Engine\Renderer\RenderPass.h"
#include "Engine\Renderer\RendererCommand.h"
#include "Platform\Vulkan\VulkanRendererAPI.h"
#include "Engine\Renderer\Shader.h"
#include "Engine\Renderer\RenderPipeline.h"

#include <vulkan\vulkan.h>

Engine::Application* Engine::Application::s_Instance = nullptr;

namespace Engine
{
	Application::Application(const std::string& title, uint32 width, uint32 height, API api)
	{
		if (s_Instance != nullptr)
			return;
		s_Instance = this;

		// init the renderer
		Renderer::Init(api);

		// create the main window
		m_Window = Engine::Window::Create(width, height, title);

	}

	Application::~Application()
	{
		m_Window->Close();
		Renderer::Destroy();
	}

	void Application::Run()
	{
		Ref<RenderPass> renderPass = RenderPass::Create();
		renderPass->m_RenderPassType = RenderPass::RenderPassType::Forword;
		renderPass->m_ClearType = RenderPass::ClearType::Color;
		renderPass->m_ClearColor = {0.0f, 0.0f, 1.0f, 1.0f};
		renderPass->GenerateRenderPass();

		Ref<Shader> shader = Shader::Create("Resorces/Shaders/TestShader.glsl");
		Ref<RenderPipeline> pipeline = RenderPipeline::Create(shader, renderPass, RenderPipeline::Topology::Triangles, false);

		// main game loop
		while (!m_Window->WantToClose())
		{
			Time::UpdateDeltaTime();
			if (!m_Window->IsMinimized())
			{
				renderPass->Execute();

			}
			m_Window->OnUpdate(); // update the window
		}

	}
}