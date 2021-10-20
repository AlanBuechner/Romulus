#include "pch.h"
#include "Application.h"
#include "Core.h"

namespace Engine
{
	Application::Application(const std::string& title, uint32 width, uint32 height)
	{
		m_Window = Engine::Window::Create(width, height, title);
	}

	void Application::Run()
	{

		while (!m_Window->IsWindowClosed())
		{
			m_Window->OnUpdate();
			//std::cout << window->IsFullScreen() << window->IsMaximized() << window->IsMinimized() << std::endl;
			CORE_INFO("{0}, {1}, {2}", m_Window->IsFullScreen(), m_Window->IsMinimized(), m_Window->IsMaximized());
		}

	}
}