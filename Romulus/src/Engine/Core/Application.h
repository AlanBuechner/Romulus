#pragma once
#include "Core.h"
#include "Window.h"
#include "LayerStack.h"
#include "ApplicationEvent.h"
#include "Event.h"

namespace Engine 
{
	class Application
	{
	public:
		Application(const std::string& title, uint32 width, uint32 height);

		virtual void OnUpdate() = 0;
		void Run();

		void PushLayer(Layer* Layer);
		void PushOverlay(Layer* Layer);

	private:
		Ref<Window> m_Window;

		LayerStack m_LayerStack;
	};

	Application* CreateApplication();


}