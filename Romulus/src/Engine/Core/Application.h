#pragma once
#include "Core.h"
#include "Window.h"

namespace Engine 
{
	class Application 
	{
	public:
		Application(const std::string& title, uint32 width, uint32 height);

		virtual void OnUpdate() = 0;
		void Run();

	private:
		Ref<Window> m_Window;
	
	};

	Application* CreateApplication();

}