#include "pch.h"
#include "Application.h"
#include "Core.h"
#include "Time.h"

Engine::Application* Engine::Application::s_Instance = nullptr;

namespace Engine
{
	Application::Application(const std::string& title, uint32 width, uint32 height, API api)
	{
		if (s_Instance != nullptr)
			return;
		s_Instance = this;

		// create the main window
		m_Window = Engine::Window::Create(width, height, title);

		Renderer::Init(api);
	}

	void Application::Run()
	{
		// main game loop
		while (!m_Window->IsWindowClosed())
		{
			m_Window->OnUpdate(); // update the window
			
		}

		Renderer::Destroy();

	}
}