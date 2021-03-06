#pragma once
#include "Core.h"
#include "ApplicationEvent.h"
#include <string>

namespace Engine
{
	class Window
	{
	protected:
		Window(uint32 width, uint32 height, const std::string& title);

	public:
		using EventCallbackFn = std::function<void(Event&)>;

		virtual void OnUpdate() = 0; // updates the window

		virtual void Close() = 0;

		virtual bool IsWindowClosed() = 0;

		inline uint32_t GetWidth() { return m_Width; }
		inline uint32_t GetHeight() { return m_Height; }
		inline const std::string& GetTitle() { return m_Title; }
		inline bool IsMinimized() { return m_Minimized; }
		inline bool IsMaximized() { return m_Maximized; }
		inline bool IsFullScreen() { return m_FullScreen; }

		virtual void SetWidth(uint32 width) = 0;
		virtual void SetHeight(uint32 height) = 0;
		virtual void Resize(uint32 width, uint32 height) = 0;
		virtual void SetTitle(const std::string& title) = 0;

		virtual void ToggleMinimize() = 0;
		virtual void ToggleMaximize() = 0;
		virtual void ToggleFullScreen() = 0;

		void SetWindowEventCallback(const EventCallbackFn& callback);

		static Ref<Window> Create(uint32 width, uint32 height, const std::string& title);


	protected:

		uint32 m_Width;
		uint32 m_Height; 

		bool m_Minimized = false;
		bool m_Maximized = false;
		bool m_FullScreen = false;

		std::string m_Title;

		EventCallbackFn m_EventCallback = nullptr;
	};
}