#pragma once
#include "Engine\Core\Core.h"
#include "Engine\Math\Math.h"
#include "Engine\Renderer\FrameBuffer.h"

#define DECLARE_CMD(x) x():CommandBuffer(CommandType::x){}

namespace Engine
{
	enum class CommandType
	{
		BeginRenderPassCommand, EndRenderPassCommand,
		SetViewAndScissorCommand
	};

	struct CommandBuffer
	{
		CommandBuffer(CommandType t) : type(t) {}
		CommandType type;
	};

	struct BeginRenderPassCommand : public CommandBuffer
	{
		DECLARE_CMD(BeginRenderPassCommand);
		Math::Vector4 color;
		Ref<FrameBuffer> buffer;
	};

	struct EndRenderPassCommand : public CommandBuffer
	{
		DECLARE_CMD(EndRenderPassCommand);
	};

}