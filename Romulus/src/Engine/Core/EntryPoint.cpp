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
	Engine::Log::Init();

	Engine::Application* app = Engine::CreateApplication();

	app->Run();

	return EXIT_SUCCESS;
}

#endif // PLATFORM_WINDOWS