#include "pch.h"
#include "WindowsWindow.h"

Engine::WindowsWindow::WindowClass Engine::WindowsWindow::WindowClass::wndClass;

namespace Engine
{

	WindowsWindow::WindowsWindow(uint32 width, uint32 height, const std::string& title) :
		Engine::Window(width, height, title)
	{
		// create a rect with the dimentions and location of the window
		RECT wr;
		wr.left = 100;
		wr.right = width + wr.left;
		wr.top = 100;
		wr.bottom = height + wr.top;

		// adjust the window rect for the styles that it has
		DWORD style = WS_OVERLAPPED | WS_THICKFRAME | WS_CAPTION | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU;
		if (AdjustWindowRect(&wr, style, FALSE) == 0)
		{
			std::cout << "Error: cannot create window";
			__debugbreak();
		}

		// creat the window
		hWnd = CreateWindow(
			WindowClass::GetName(), GetWStr(title.c_str()), style,
			CW_USEDEFAULT, CW_USEDEFAULT, wr.right - wr.left, wr.bottom - wr.top,
			nullptr, nullptr, WindowClass::GetInstance(), this
		);

		// check if the windows was created successfuly
		CORE_ASSERT(hWnd != nullptr, "Failed to create window");
		// create the swap chain
		m_SwapChain = SwapChain::Create(width, height, this);

		// show the window
		ShowWindow(hWnd, SW_SHOW);

	}

	WindowsWindow::~WindowsWindow()
	{
		Close();
	}

	void WindowsWindow::OnUpdate()
	{
		// swap the frame buffers
		if(!m_Minimized)
			m_SwapChain->Swap();

		// pull events
		MSG msg;
		BOOL gResult = PeekMessage(&msg, hWnd, 0, 0, PM_REMOVE);
		if (gResult != 0) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if(!m_Minimized)
			m_SwapChain->Update();
	}

	void WindowsWindow::Close()
	{
		DestroyWindow(hWnd);
		hWnd = nullptr;
	}

	void WindowsWindow::SetWidth(uint32 width)
	{
		if (m_FullScreen || m_Maximized || m_Minimized) return;
		SetWindowPos(hWnd, nullptr, 0, 0, width, m_Height, 0);
	}

	void WindowsWindow::SetHeight(uint32 height)
	{
		if (m_FullScreen || m_Maximized || m_Minimized) return;
		SetWindowPos(hWnd, nullptr, 0, 0, m_Width, height, 0);
	}

	void WindowsWindow::Resize(uint32 width, uint32 height)
	{
		if (m_FullScreen || m_Maximized || m_Minimized) return;
		SetWindowPos(hWnd, nullptr, 0, 0, width, height, 0);
	}

	void WindowsWindow::SetTitle(const std::string& title)
	{
		SetWindowTextA(hWnd, title.c_str());
	}

	void WindowsWindow::ToggleMinimize()
	{
		ShowWindow(hWnd, m_Minimized ? (m_Maximized ? SW_SHOWMAXIMIZED : SW_SHOWNORMAL) : SW_SHOWMINIMIZED);
	}

	void WindowsWindow::ToggleMaximize()
	{
		ShowWindow(hWnd, m_Maximized ? SW_SHOWNORMAL : SW_SHOWMAXIMIZED);
	}

	WINDOWPLACEMENT g_wpPrev = { sizeof(g_wpPrev) };
	void WindowsWindow::ToggleFullScreen()
	{
		DWORD dwStyle = GetWindowLong(hWnd, GWL_STYLE);
		if (!m_FullScreen) 
		{
			MONITORINFO mi = { sizeof(mi) };
			if (GetWindowPlacement(hWnd, &g_wpPrev) && GetMonitorInfo(MonitorFromWindow(hWnd, MONITOR_DEFAULTTOPRIMARY), &mi))
			{
				SetWindowLong(hWnd, GWL_STYLE,dwStyle & ~WS_OVERLAPPEDWINDOW);
				SetWindowPos(hWnd, HWND_TOP,
					mi.rcMonitor.left, mi.rcMonitor.top,
					mi.rcMonitor.right - mi.rcMonitor.left,
					mi.rcMonitor.bottom - mi.rcMonitor.top,
					SWP_NOOWNERZORDER | SWP_FRAMECHANGED
				);

				m_FullScreen = true;
				m_Minimized = false;
			}
		}
		else 
		{
			SetWindowLong(hWnd, GWL_STYLE, dwStyle | WS_OVERLAPPEDWINDOW);
			SetWindowPlacement(hWnd, &g_wpPrev);

			ShowWindow(hWnd, m_Minimized ? SW_SHOWMINIMIZED : (m_Maximized ? SW_SHOWMAXIMIZED : SW_SHOWNORMAL));
			m_FullScreen = false;
		}
	}

	LRESULT WINAPI WindowsWindow::HandleEventSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		if (msg == WM_NCCREATE)
		{
			// get the data the was sent from CreateWindow
			const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
			WindowsWindow* const pWnd = static_cast<WindowsWindow*>(pCreate->lpCreateParams); // get the window

			SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd)); // set user data on the window to contain the Window class
			SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(WindowsWindow::HandleEventThunk)); // change the event handler to handle event thunk

			return pWnd->HandleEvent(hWnd, msg, wParam, lParam); // handle the event
		}

		return DefWindowProc(hWnd, msg, wParam, lParam);
	}

	LRESULT WINAPI WindowsWindow::HandleEventThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		 // get a pointer to the window
		WindowsWindow* const pWnd = reinterpret_cast<WindowsWindow*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
		return pWnd->HandleEvent(hWnd, msg, wParam, lParam); // call handle event for that window
	}

	LRESULT WindowsWindow::HandleEvent(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{

		switch (msg)
		{
			// keyboard events
		case WM_SYSKEYDOWN:
		case WM_KEYDOWN: // key down
			break;
		case WM_SYSKEYUP:
		case WM_KEYUP: // key up
			break;
		case WM_CHAR:
			break;

			// mouse
		case WM_MOUSEMOVE: // mouse move
			break;

		case WM_LBUTTONDOWN: // left mouse down
			break;
		case WM_RBUTTONDOWN: // right mouse down
			break;
		case WM_MBUTTONDOWN: // middle mouse down
			break;

		case WM_LBUTTONUP: // left moues up
			break;
		case WM_RBUTTONUP: // right mouse up
			break;
		case WM_MBUTTONUP: // middle mouse up
			break;


			// window events
		case WM_SIZE: // windows resize event
		{
			switch (wParam)
			{
			case SIZE_MAXIMIZED:
				m_Maximized = true;
				m_Minimized = false;
				break;
			case SIZE_MINIMIZED:
				m_Minimized = true;
				break;
			case SIZE_RESTORED:
				m_Minimized = false;
				m_Maximized = false;
				break;
			
			default:
				break;
			
			}

			if (!m_Minimized)
			{
				// set the width and hight
				m_Width = LOWORD(lParam);
				m_Height = HIWORD(lParam);
				// resize swap chain
				m_SwapChain->Resize(m_Width, m_Height);
			}
			break;
		}
		case WM_KILLFOCUS: // window remove focuse event
			break;
		case WM_CLOSE: // window close event
			m_WantToClose = true;
			return true;
			break;
		}
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}





	// ----------------------- window class ----------------------- //

	WindowsWindow::WindowClass::WindowClass() :
		hInst(GetModuleHandle(nullptr))
	{
		WNDCLASSEX wc = { 0 };
		wc.cbSize = sizeof(wc);
		wc.style = CS_OWNDC;
		wc.lpfnWndProc = WindowsWindow::HandleEventSetup;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = GetInstance();
		wc.hIcon = nullptr;
		wc.hIconSm = nullptr;
		wc.hbrBackground = nullptr;
		wc.hCursor = nullptr;
		wc.lpszMenuName = nullptr;
		wc.lpszClassName = GetName();

		RegisterClassEx(&wc);
	}

	WindowsWindow::WindowClass::~WindowClass()
	{
		UnregisterClass(wndClassName, GetInstance());
	}


}