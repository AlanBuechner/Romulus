#include "pch.h"
#include "Core.h"
#include "Window.h"
#include "Engine\Math\Math.h"
#include "Cursor.h"
#include "Log.h"
#include "Application.h"
#include <Engine/Util/Performance.h>

#ifdef PLATFORM_WINDOWS

extern Engine::Application* Engine::CreateApplication();
	
int main(int argc, char** argv)
{
	// initialize the loging system
	Engine::Log::Init();


	// create an application
	Engine::Instrumentor::Get().BeginSession("Startup", "startup.json");
	Engine::Application* app = Engine::CreateApplication();
	Engine::Instrumentor::Get().EndSession();

	Engine::Instrumentor::Get().BeginSession("Runtime", "runtime.json");
	app->Run();
	Engine::Instrumentor::Get().EndSession();

	return EXIT_SUCCESS;
}

#endif // PLATFORM_WINDOWS