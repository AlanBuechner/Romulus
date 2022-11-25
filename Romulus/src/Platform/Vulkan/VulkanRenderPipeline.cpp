#include "pch.h"
#include "VulkanRenderPipeline.h"
#include "VulkanRendererAPI.h"
#include "VulkanSwapChain.h"
#include "Engine\Renderer\RendererCommand.h"
#include "Engine\Core\Application.h"

#include "VulkanRenderPass.h"
#include "VulkanShader.h"

namespace Engine
{

	VulkanRenderPipeline::VulkanRenderPipeline(Ref<Shader> shader, Ref<RenderPass> renderPass, Topology topology, bool wireframe)
	{
		VulkanRendererAPI& api = *(VulkanRendererAPI*)RendererCommand::GetApiInstance();
		VulkanShader& vkShader = *(VulkanShader*)shader.get();
		SwapChain& swapChain = Application::GetWindow()->GetSwapChain();

		// vertex input state
		VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
		vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		vertexInputInfo.vertexBindingDescriptionCount = 0;
		vertexInputInfo.pVertexBindingDescriptions = nullptr; // Optional
		vertexInputInfo.vertexAttributeDescriptionCount = 0;
		vertexInputInfo.pVertexAttributeDescriptions = nullptr; // Optional

		// input assembly
		VkPrimitiveTopology t;
		switch (topology)
		{
		case Engine::RenderPipeline::Topology::Points:
			t = VK_PRIMITIVE_TOPOLOGY_POINT_LIST;
			break;
		case Engine::RenderPipeline::Topology::Lines:
			t = VK_PRIMITIVE_TOPOLOGY_LINE_LIST;
			break;
		case Engine::RenderPipeline::Topology::Triangles:
			t = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
			break;
		default:
			CORE_ASSERT(false, "invlid topology type");
			break;
		}

		VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
		inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		inputAssembly.topology = t;
		inputAssembly.primitiveRestartEnable = VK_FALSE;

		// viewport and scissor
		VkViewport viewport{};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = (float)swapChain.GetWidth();
		viewport.height = (float)swapChain.GetHeight();
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;

		VkRect2D scissor{};
		scissor.offset = { 0, 0 };
		scissor.extent = { swapChain.GetWidth(), swapChain.GetHeight() };

		VkPipelineViewportStateCreateInfo viewportState{};
		viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		viewportState.viewportCount = 1;
		viewportState.pViewports = &viewport;
		viewportState.scissorCount = 1;
		viewportState.pScissors = &scissor;

		// rasterizer
		VkPipelineRasterizationStateCreateInfo rasterizer{};
		rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		rasterizer.depthClampEnable = VK_FALSE;
		rasterizer.rasterizerDiscardEnable = VK_FALSE;
		rasterizer.polygonMode = wireframe ? VK_POLYGON_MODE_LINE : VK_POLYGON_MODE_FILL;
		rasterizer.lineWidth = 1.0f;
		rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
		rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
		rasterizer.depthBiasEnable = VK_FALSE;
		rasterizer.depthBiasConstantFactor = 0.0f; // Optional
		rasterizer.depthBiasClamp = 0.0f; // Optional
		rasterizer.depthBiasSlopeFactor = 0.0f; // Optional

		// multisampling
		VkPipelineMultisampleStateCreateInfo multisampling{};
		multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		multisampling.sampleShadingEnable = VK_FALSE;
		multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
		multisampling.minSampleShading = 1.0f; // Optional
		multisampling.pSampleMask = nullptr; // Optional
		multisampling.alphaToCoverageEnable = VK_FALSE; // Optional
		multisampling.alphaToOneEnable = VK_FALSE; // Optional

		// create shader stage info
		std::vector<VkPipelineShaderStageCreateInfo> shaderStageInfo(vkShader.GetShaderModules().size());
		uint32 shaderStageIndex = 0;
		for (auto s : vkShader.GetShaderModules())
		{
			VkShaderStageFlagBits shaderStage;
			switch (s.first)
			{
			case Shader::ShaderType::Vertex: shaderStage = VK_SHADER_STAGE_VERTEX_BIT; break;
			case Shader::ShaderType::Pixle: shaderStage = VK_SHADER_STAGE_FRAGMENT_BIT; break;
			case Shader::ShaderType::Geometry: shaderStage = VK_SHADER_STAGE_GEOMETRY_BIT; break;
			default:
				CORE_ASSERT(false, "not a valid shader type");
				return;
			}

			shaderStageInfo[shaderStageIndex].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
			shaderStageInfo[shaderStageIndex].stage = shaderStage;
			shaderStageInfo[shaderStageIndex].module = s.second;
			shaderStageInfo[shaderStageIndex].pName = "main";
			shaderStageIndex++;
		}

		// color blending
		VkPipelineColorBlendAttachmentState colorBlendAttachment{};
		colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
		colorBlendAttachment.blendEnable = VK_FALSE;
		colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
		colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
		colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD; // Optional
		colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
		colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
		colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD; // Optional

		VkPipelineColorBlendStateCreateInfo colorBlending{};
		colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		colorBlending.logicOpEnable = VK_FALSE;
		colorBlending.logicOp = VK_LOGIC_OP_COPY; // Optional
		colorBlending.attachmentCount = 1;
		colorBlending.pAttachments = &colorBlendAttachment;
		colorBlending.blendConstants[0] = 0.0f; // Optional
		colorBlending.blendConstants[1] = 0.0f; // Optional
		colorBlending.blendConstants[2] = 0.0f; // Optional
		colorBlending.blendConstants[3] = 0.0f; // Optional

		// dynamic states
		std::vector<VkDynamicState> dynamicStates = {
			VK_DYNAMIC_STATE_VIEWPORT,
		};

		VkPipelineDynamicStateCreateInfo dynamicState{};
		dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
		dynamicState.dynamicStateCount = (uint32)dynamicStates.size();
		dynamicState.pDynamicStates = dynamicStates.data();

		// depth stencel testing
		VkPipelineDepthStencilStateCreateInfo dsStateInfo{};
		dsStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
		dsStateInfo.minDepthBounds = 0.0f;
		dsStateInfo.maxDepthBounds = 1.0f;
		dsStateInfo.depthTestEnable = VK_TRUE;
		dsStateInfo.depthWriteEnable = VK_TRUE;
		dsStateInfo.depthBoundsTestEnable = VK_FALSE;
		dsStateInfo.stencilTestEnable = VK_FALSE;
		dsStateInfo.depthCompareOp = VK_COMPARE_OP_LESS;

		// pipeline layout
		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = 0; // Optional
		pipelineLayoutInfo.pSetLayouts = nullptr; // Optional
		pipelineLayoutInfo.pushConstantRangeCount = 0; // Optional
		pipelineLayoutInfo.pPushConstantRanges = nullptr; // Optional

		CORE_ASSERT(vkCreatePipelineLayout(api.GetDevice(), &pipelineLayoutInfo, nullptr, &m_PipelineLayout) == VK_SUCCESS,
			"Failed to create pipeline layout");

		// create pipeline
		VkGraphicsPipelineCreateInfo pipelineInfo{};
		pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		pipelineInfo.pVertexInputState = &vertexInputInfo;
		pipelineInfo.pInputAssemblyState = &inputAssembly;
		pipelineInfo.pViewportState = &viewportState;
		pipelineInfo.pRasterizationState = &rasterizer;
		pipelineInfo.pMultisampleState = &multisampling;
		pipelineInfo.stageCount = (uint32_t)shaderStageInfo.size();
		pipelineInfo.pStages = shaderStageInfo.data();
		pipelineInfo.pColorBlendState = &colorBlending;
		pipelineInfo.pDepthStencilState = &dsStateInfo;
		pipelineInfo.layout = m_PipelineLayout;
		pipelineInfo.renderPass = ((VulkanRenderPass*)renderPass.get())->GetRenderPass();
		pipelineInfo.subpass = 0;

		pipelineInfo.basePipelineIndex = -1;
		pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

		CORE_ASSERT(vkCreateGraphicsPipelines(api.GetDevice(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &m_Pipeline) == VK_SUCCESS,
			"Failed to create Render pipeline");
	}

	VulkanRenderPipeline::~VulkanRenderPipeline()
	{
		VulkanRendererAPI& api = *(VulkanRendererAPI*)RendererCommand::GetApiInstance();
		vkDestroyPipelineLayout(api.GetDevice(), m_PipelineLayout, nullptr);
		vkDestroyPipeline(api.GetDevice(), m_Pipeline, nullptr);
	}

}