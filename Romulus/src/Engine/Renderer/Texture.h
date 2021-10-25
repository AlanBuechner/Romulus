#pragma once
#include "Engine\Core\Core.h"

namespace Engine
{
	enum class TextureFormat
	{
		DepthStencil, Depth, Stencil,
		Screen // for internal use only
	};

	class Texture
	{
	public:

		uint32 GetWidth() const { return m_Width; }
		uint32 GetHeight() const { return m_Height; }
		TextureFormat GetFormat() const { return m_Format; }

		static Ref<Texture> Create(uint32 width, uint32 height, TextureFormat format, void* data = nullptr);

	protected:
		uint32 m_Width, m_Height;
		TextureFormat m_Format;

	};
}