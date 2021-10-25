#pragma once
#include "Engine\Core\Core.h"
#include "FrameBuffer.h"

namespace Engine
{
	class SwapChain
	{
	public:

		virtual void Resize(uint32 width, uint32 height) = 0;
		virtual Ref<FrameBuffer> GetFrontBuffer() = 0;

		static Scope<SwapChain> Create(uint32 width, uint32 height, void* window);
	};
}