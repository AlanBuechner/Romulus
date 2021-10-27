#pragma once
#include "Engine\Core\Core.h"
#include "FrameBuffer.h"

namespace Engine
{
	class SwapChain
	{
	public:

		void Resize(uint32 width, uint32 height);
		virtual void Regenerate() = 0;
		virtual Ref<FrameBuffer> GetBackBuffer() = 0;

		virtual void Swap() = 0;
		virtual void GetNextImage() = 0;

		static Scope<SwapChain> Create(uint32 width, uint32 height, void* window);

	protected:
		bool m_Regenerate = true;
		uint32 m_NewWidth, m_NewHeight;
	};
}