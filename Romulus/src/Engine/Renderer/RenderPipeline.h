#pragma once
#include "Engine\Core\Core.h"
#include "Shader.h"
#include "RenderPass.h"

namespace Engine
{
	class RenderPipeline
	{
	public:
		enum class Topology
		{
			Points,
			Lines,
			Triangles
		};

	public:
		static Ref<RenderPipeline> Create(Ref<Shader> shader, Ref<RenderPass> renderPass, Topology topology, bool wireframe);
	};
}