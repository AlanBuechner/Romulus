#include "pch.h"
#include "FrameBuffer.h"
#include "RendererCommand.h"

#include "Platform\Vulkan\VulkanFrameBuffer.h"

namespace Engine
{
	Ref<FrameBuffer> Engine::FrameBuffer::Create(uint32 width, uint32 height, const std::vector<FrameBufferAttachments>& attachments)
	{

		switch (RendererCommand::GetApi())
		{
		case API::Vulkan:
			return CreateRef<VulkanFrameBuffer>(width, height, attachments);
		default:
			break;
		}

		return Ref<FrameBuffer>();
	}

}