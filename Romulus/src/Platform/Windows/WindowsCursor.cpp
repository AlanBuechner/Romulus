#include "pch.h"
#include "Engine\Core\Core.h"
#include "Engine\Core\Cursor.h"

#if defined(PLATFORM_WINDOWS)

#include "WindowsWindow.h"
#include <Windows.h>

namespace Engine
{

	void Cursor::ConstrainToWindow(Ref<Window> window)
	{
		HWND& hWnd = ((WindowsWindow*)window.get())->hWnd;

		RECT winRect;
		GetWindowRect(hWnd, &winRect);

		ClipCursor(&winRect);
	}

	void Cursor::Visibility(bool v)
	{
		ShowCursor(v);
	}

}


#endif 