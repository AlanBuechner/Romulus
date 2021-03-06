#include "pch.h"
#include "Application.h"
#include "Time.h"
#include "Layer.h"
#include "KeyEvent.h"
#include "Engine/Util/Performance.h"

Engine::Application* Engine::Application::s_Instance = nullptr;

namespace Engine
{
	Application::Application(const std::string& title, uint32 width, uint32 height)
	{
		if (s_Instance != nullptr)
			return;
		s_Instance = this;

		// create the main window
		CREATE_PROFILE_FUNCTIONI();
		auto timer = CREATE_PROFILEI();
		timer.Start("Create Window");
		m_Window = Engine::Window::Create(width, height, title);
		m_Window->SetWindowEventCallback(BIND_EVENT_FN(&Application::OnEvent));
		timer.End();

	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
	}

	void Application::PushOverlay(Layer* layer)
	{
		m_LayerStack.PushOverlay(layer);
	}


	void Application::OnEvent(Event& e)
	{
		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); )
		{
			(*--it)->OnEvent(e);
			if (e.m_Handled)
				break;
		}

		if (e.GetEventType() == EventType::KeyPressed)
		{
			CORE_INFO("{0}", (char)((KeyPressedEvent&)e).GetKeyCode());
		}

		delete &e;

	}


	void Application::Run()
	{
		// main game loop
		
		while (!m_Window->IsWindowClosed())
		{
			CREATE_PROFILE_SCOPEI("Frame");
			for (Layer* layer : m_LayerStack)
				layer->OnUpdate();
			m_Window->OnUpdate();
		}
	}


}