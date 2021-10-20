#include "pch.h"
#include "Application.h"
#include "Core.h"
#include "Time.h"

namespace Engine
{
	Application::Application(const std::string& title, uint32 width, uint32 height)
	{
		// create the main window
		m_Window = Engine::Window::Create(width, height, title);
	}

	void Application::Run()
	{
		// main game loop
		while (!m_Window->IsWindowClosed())
		{
			m_Window->OnUpdate(); // update the window
			CORE_INFO("{0}", Time::GetFPS());
		}

	}
}