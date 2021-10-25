#include "pch.h"
#include "Texture.h"
#include "RendererCommand.h"

#include "Platform\Vulkan\VulkanTexture.h"

namespace Engine
{
	Ref<Texture> Texture::Create(uint32 width, uint32 height, TextureFormat format, void* data)
	{
		switch (RendererCommand::GetApi())
		{
		case API::Vulkan:
			return CreateRef<VulkanTexture>(width, height, format, data);
		default:
			break;
		}
		return Ref<Texture>();
	}

}