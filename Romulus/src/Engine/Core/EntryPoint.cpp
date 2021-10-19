#include "pch.h"
#include "Core.h"
#include "Window.h"
#include "Engine\Math\Math.h"

#ifdef PLATFORM_WINDOWS

int main(int argc, char** argv)
{
	Engine::Ref<Engine::Window> window = Engine::Window::Create(1080,720, "Romulus");

	std::cout << "hello world" << std::endl;

	window->ToggleMinimize();
	window->Resize(100,100);

	while (!window->IsWindowClosed())
	{
		window->OnUpdate();
		std::cout << window->IsFullScreen() << window->IsMaximized() << window->IsMinimized() << std::endl;
	}

	return EXIT_SUCCESS;
}

#endif // PLATFORM_WINDOWS