#include "pch.h"
#include "Application.h"
#include "Core.h"
#include "Time.h"
#include "Layer.h"

namespace Engine
{
	Application::Application(const std::string& title, uint32 width, uint32 height)
	{
		m_Window = Engine::Window::Create(width, height, title);
	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
	}

	void Application::PushOverlay(Layer* layer)
	{
		m_LayerStack.PushOverlay(layer);
	}


//	void Application::OnEvent(Event& e)
//	{
//		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); )
//		{
//			(*--it)->OnEvent(e);
//			if (e.handled)
//				break;
//		}
//
//	}


	void Application::Run()
	{

		while (!m_Window->IsWindowClosed())
		{
			m_Window->OnUpdate();
//			std::cout << window->IsFullScreen() << window->IsMaximized() << window->IsMinimized() << std::endl;
			CORE_INFO("{0}, {1}, {2}", m_Window->IsFullScreen(), m_Window->IsMinimized(), m_Window->IsMaximized());
			CORE_INFO("{0}", Time::GetFPS());
		}
		
		for (Layer* layer : m_LayerStack)
			layer->OnUpdate();
	
	
	}



}