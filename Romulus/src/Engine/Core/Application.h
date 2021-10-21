#pragma once
#include "Core.h"
#include "Window.h"
#include "Engine\Renderer\Renderer.h"

namespace Engine 
{
	class Application 
	{
		static Application* s_Instance;

	public:
		Application(const std::string& title, uint32 width, uint32 height, API api);

		static Application* Get() { return s_Instance; }
		static Ref<Window> GetWindow() { return s_Instance->m_Window; }

		virtual void OnUpdate() = 0;
		void Run();

	private:
		Ref<Window> m_Window;
	
	};

	Application* CreateApplication();

}