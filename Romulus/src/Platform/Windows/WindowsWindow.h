#pragma once
#include "Engine\Core\Core.h"
#include "Engine\Core\Window.h"

#include <Windows.h>

namespace Engine
{
	class Cursor;
}

namespace Engine
{
	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(uint32 width, uint32 height, const std::string& title);
		~WindowsWindow();

		virtual void OnUpdate() override; // updates the window

		virtual void Close() override;

		virtual bool IsWindowClosed() override { return hWnd == nullptr; }

		virtual void SetWidth(uint32 width) override;
		virtual void SetHeight(uint32 height) override;
		virtual void Resize(uint32 width, uint32 height) override;
		virtual void SetTitle(const std::string& title) override;

		virtual void ToggleMinimize() override;
		virtual void ToggleMaximize() override;
		virtual void ToggleFullScreen() override;

	private:
		static LRESULT WINAPI HandleEventSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam); // handles seting up events
		static LRESULT WINAPI HandleEventThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam); // sends events to the corisponding window to be handeled
		LRESULT HandleEvent(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	private:
		HWND hWnd; // handle to the window

	private:

		class WindowClass
		{
		public:
			static const LPCWSTR GetName() noexcept { return wndClassName; }
			static HINSTANCE GetInstance() noexcept { return wndClass.hInst; }

		private:
			WindowClass();
			~WindowClass();
			WindowClass(const WindowClass&) = delete;
			static constexpr const LPCWSTR wndClassName = TEXT("Window");
			static WindowClass wndClass;
			HINSTANCE hInst;
		};


		friend Cursor;
	};
}