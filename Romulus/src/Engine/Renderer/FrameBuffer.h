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

		virtual void BindToRenderPass(const RenderPass& renderPass) = 0;
		inline uint32 GetWidth() { return m_Width; }
		inline uint32 GetHeight() { return m_Height; }

		static Ref<FrameBuffer> Create(uint32 width, uint32 height, const std::vector<FrameBufferAttachments>& attachments);

	protected:
		std::vector<FrameBufferAttachments> m_Attachments;
		uint32 m_Width, m_Height;
	};
}