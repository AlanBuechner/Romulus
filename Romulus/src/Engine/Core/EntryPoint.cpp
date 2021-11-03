#include "pch.h"
#include "Core.h"
#include "Window.h"
#include "Engine\Math\Math.h"
#include "Cursor.h"
#include "Log.h"
#include "Application.h"

#ifdef PLATFORM_WINDOWS

extern Engine::Application* Engine::CreateApplication();
	
int main(int argc, char** argv)
{
	// initialize the loging system
	Engine::Log::Init();

	// create an application
	Engine::Application* app = Engine::CreateApplication();

	app->Run(); // run the application

	return EXIT_SUCCESS;
}

#endif // PLATFORM_WINDOWS