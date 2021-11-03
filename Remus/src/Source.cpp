#include "Engine/Core/Application.h"

namespace Engine
{

	class App : public Application
	{
	public:
		App() :
			Application("Romulus", 1024, 768)
		{};

		virtual void OnUpdate() override {};
	};

	Application* CreateApplication()
	{
		return (Application*)new App();
	}

}