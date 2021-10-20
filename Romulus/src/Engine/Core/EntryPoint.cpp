#include "pch.h"
#include "Core.h"
#include "Window.h"
#include "Engine\Math\Math.h"
#include "Cursor.h"
#include "Log.h"

#ifdef PLATFORM_WINDOWS
	
int main(int argc, char** argv)
{
	Engine::Log::Init();
	Engine::Ref<Engine::Window> window = Engine::Window::Create(1080,720, "Romulus");

	std::cout << "Hello World!" << std::endl;

	while (!window->IsWindowClosed())
	{
		window->OnUpdate();
		//std::cout << window->IsFullScreen() << window->IsMaximized() << window->IsMinimized() << std::endl;
		CORE_INFO("{0}, {1}, {2}", window->IsFullScreen(), window->IsMinimized(), window->IsMaximized());
	}

	return EXIT_SUCCESS;
}

#endif // PLATFORM_WINDOWS