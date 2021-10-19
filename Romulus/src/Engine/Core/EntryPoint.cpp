#include "pch.h"
#include "Core.h"
#include "Window.h"
#include "Engine\Math\Math.h"

#ifdef PLATFORM_WINDOWS

int main(int argc, char** argv)
{
	Engine::Ref<Engine::Window> window = Engine::Window::Create(1080,720, "Romulus");

	std::cout << "hello world" << std::endl;

	//window->ToggleMaximize();
	//window->ToggleFullScreen();
	//window->ToggleFullScreen();


	while (!window->IsWindowClosed())
	{
		window->OnUpdate();
	}

	return EXIT_SUCCESS;
}

#endif // PLATFORM_WINDOWS