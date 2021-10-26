#pragma once
#include "Engine\Core\Core.h"
#include "Engine\Math\Math.h"
#include "FrameBuffer.h"

namespace Engine
{

	class RenderPass
	{
	public:
		enum class ClearType
		{
			None, Color
		};

		enum class RenderPassType
		{
			Forword
		};

	public:

		virtual void GenerateRenderPass() = 0;

		virtual void Run() = 0;

		static Ref<RenderPass> Create();

	public:
		RenderPassType m_RenderPassType;
		ClearType m_ClearType;
		Math::Vector4 m_ClearColor;
		Ref<FrameBuffer> m_DestinationBuffer;

	};
}