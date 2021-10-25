#pragma once
#include "Core.h"
#include "Engine\Renderer\SwapChain.h"
#include <string>

namespace Engine
{
	class Window
	{
	protected:
		Window(uint32 width, uint32 height, const std::string& title);

	public:

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

		inline SwapChain& GetSwapChain() { return *m_SwapChain.get(); }

		static Ref<Window> Create(uint32 width, uint32 height, const std::string& title);

	protected:

		uint32 m_Width;
		uint32 m_Height;
		Scope<SwapChain> m_SwapChain;


		bool m_Minimized = false;
		bool m_Maximized = false;
		bool m_FullScreen = false;

		std::string m_Title;
	};
}