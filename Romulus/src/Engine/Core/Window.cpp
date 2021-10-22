#include "pch.h"
#include "Window.h"

#if defined(PLATFORM_WINDOWS)
#include <Platform\Windows\WindowsWindow.h>
#endif

namespace Engine
{
	Window::Window(uint32 width, uint32 height, const std::string& title) :
		m_Width(width), m_Height(height), m_Title(title)
	{}

	Ref<Window> Window::Create(uint32 width, uint32 height, const std::string& title)
	{
#if defined(PLATFORM_WINDOWS)
		return CreateRef<WindowsWindow>(width, height, title);
#endif
	}

}