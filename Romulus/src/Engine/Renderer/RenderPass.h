#pragma once
#include "Engine\Core\Core.h"
#include "Engine\Math\Math.h"
#include "FrameBuffer.h"
#include "CommandBuffers.h"

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

		template <typename T>
		void RecoredCommand(const T& command)
		{
			m_Commands.push_back(CreateRef<T>(command));
		}

		virtual void GenerateRenderPass() = 0;

		virtual void Execute() = 0;

		static Ref<RenderPass> Create();

	public:
		RenderPassType m_RenderPassType;
		ClearType m_ClearType;
		Math::Vector4 m_ClearColor;
		Ref<FrameBuffer> m_DestinationBuffer;

		std::vector<Ref<CommandBuffer>> m_Commands;

	};
}