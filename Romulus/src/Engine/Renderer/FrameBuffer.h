#pragma once
#include "Engine\Core\Core.h"
#include "Texture.h"

namespace Engine
{
	class RenderPass;
}

namespace Engine
{
	struct FrameBufferAttachments
	{
		TextureFormat format;
		Ref<Texture> texture; // leave as null to generate one
	};

	class FrameBuffer
	{
	public:

		inline std::vector<FrameBufferAttachments>& GetAttachments() { return m_Attachments; }

		virtual void BindToRenderPass(RenderPass& renderPass) = 0;

		static Ref<FrameBuffer> Create(uint32 width, uint32 height, const std::vector<FrameBufferAttachments>& attachments);

	protected:
		std::vector<FrameBufferAttachments> m_Attachments;
	};
}