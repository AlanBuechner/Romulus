#include "pch.h"
#include "VulkanTexture.h"
#include "VulkanRendererAPI.h"
#include "Engine\Renderer\RendererCommand.h"

namespace Engine
{
	VulkanTexture::VulkanTexture(uint32 width, uint32 height, TextureFormat format, void* data) 
	{
		VulkanRendererAPI& api = *(VulkanRendererAPI*)RendererCommand::GetApiInstance();

		m_Width = width;
		m_Height = height;
		m_Format = format;

		// image fomats
		const static std::vector<VkFormat> depthStencilFormats{
			VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT, VK_FORMAT_D16_UNORM_S8_UINT, // depth and stencil format
		};

		const static std::vector<VkFormat> depthFormats{
			VK_FORMAT_D32_SFLOAT, VK_FORMAT_D16_UNORM
		};

		// get the potential vulkan formats
		const std::vector<VkFormat>* formats = nullptr;
		switch (format)
		{
		case Engine::TextureFormat::DepthStencil:
			formats = &depthStencilFormats;
			break;
		case Engine::TextureFormat::Depth:
			formats = &depthFormats;
			break;
		case Engine::TextureFormat::Stencil:
			m_ImageFormat = VK_FORMAT_S8_UINT;
			break;
		default:
			break;
		}


		if (formats != nullptr)
		{
			// find the best fiting format
			for (auto f : *formats)
			{
				VkFormatProperties fomatProps;
				vkGetPhysicalDeviceFormatProperties(api.GetPhysicalDevice(), f, &fomatProps);
				if (fomatProps.optimalTilingFeatures & VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT)
				{
					m_ImageFormat = f;
					break;
				}
			}
		}

		CORE_ASSERT(m_ImageFormat != VK_FORMAT_UNDEFINED, "failed to find a suported depth stencil format");

		VkImageUsageFlagBits usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
		switch (format)
		{
		case Engine::TextureFormat::DepthStencil:
			usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
			break;
		case Engine::TextureFormat::Depth:
			usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
			break;
		case Engine::TextureFormat::Stencil:
			usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
			break;
		default:
			break;
		}

		// create image buffer 
		VkImageCreateInfo dsCreateInfo{};
		dsCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		dsCreateInfo.flags = 0;
		dsCreateInfo.imageType = VK_IMAGE_TYPE_2D;
		dsCreateInfo.format = m_ImageFormat;
		dsCreateInfo.extent = { width, height, 1 };
		dsCreateInfo.mipLevels = 1;
		dsCreateInfo.arrayLayers = 1;
		dsCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;
		dsCreateInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
		dsCreateInfo.usage = usage;
		dsCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		dsCreateInfo.queueFamilyIndexCount = VK_QUEUE_FAMILY_IGNORED; // ignored if image sharing mode is exclusive
		dsCreateInfo.pQueueFamilyIndices = nullptr; // ignored if image sharing mode is exclusive
		dsCreateInfo.initialLayout = m_Layout;

		CORE_ASSERT(vkCreateImage(api.GetDevice(), &dsCreateInfo, nullptr, &m_ImageBuffer) == VK_SUCCESS,
			"failed to create depth stencil buffer");

		VkMemoryRequirements dsMemReq{};
		vkGetImageMemoryRequirements(api.GetDevice(), m_ImageBuffer, &dsMemReq);

		// find where to allocate memory for the image
		uint32 memIndex = UINT32_MAX;
		uint32 requiredProps = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
		VkPhysicalDeviceMemoryProperties memProps = api.GetMemoryProps();
		for (uint32 i = 0; i < memProps.memoryTypeCount; i++)
		{
			if (dsMemReq.memoryTypeBits & BIT(i))
			{
				if ((memProps.memoryTypes[i].propertyFlags & requiredProps) == requiredProps)
				{
					memIndex = i;
					break;
				}
			}
		}
		CORE_ASSERT(memIndex != UINT32_MAX, "failed to find memory type index for depth stencil buffer");

		// allocate memory for the image
		VkMemoryAllocateInfo dsAllocInfo{};
		dsAllocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		dsAllocInfo.allocationSize = dsMemReq.size;
		dsAllocInfo.memoryTypeIndex = memIndex;
		CORE_ASSERT(vkAllocateMemory(api.GetDevice(), &dsAllocInfo, nullptr, &m_ImageMemory) == VK_SUCCESS,
			"failed to allocate memory for depth stencil buffer");

		// bind the depth stencil image
		vkBindImageMemory(api.GetDevice(), m_ImageBuffer, m_ImageMemory, 0);

		// create depth stencil image view
		VkImageViewCreateInfo dsViewCreateInfo{};
		dsViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		dsViewCreateInfo.image = m_ImageBuffer;
		dsViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		dsViewCreateInfo.format = m_ImageFormat;
		dsViewCreateInfo.components = { VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY,
										VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY };;
		dsViewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT;
		dsViewCreateInfo.subresourceRange.baseMipLevel = 0;
		dsViewCreateInfo.subresourceRange.levelCount = 1;
		dsViewCreateInfo.subresourceRange.baseArrayLayer = 0;
		dsViewCreateInfo.subresourceRange.layerCount = 1;

		CORE_ASSERT(vkCreateImageView(api.GetDevice(), &dsViewCreateInfo, nullptr, &m_ImageView) == VK_SUCCESS,
			"failed to create depth stencil image view");

		// upload data to the image

	}

	VulkanTexture::~VulkanTexture()
	{
		VulkanRendererAPI& api = *(VulkanRendererAPI*)RendererCommand::GetApiInstance();
		vkDestroyImageView(api.GetDevice(), m_ImageView, nullptr);
		vkFreeMemory(api.GetDevice(), m_ImageMemory, nullptr);
		vkDestroyImage(api.GetDevice(), m_ImageBuffer, nullptr);
	}

	void VulkanTexture::CreateImage(uint32 width, uint32 height, VkImage image, VkImageView view, VkFormat vFormat, VkDeviceMemory memory, VkImageLayout vLayout, TextureFormat format)
	{
		m_Width = width;
		m_Height = height;
		m_Format = format;
		m_ImageBuffer = image;
		m_ImageView = view;
		m_ImageFormat = vFormat;
		m_ImageMemory = memory;
		m_Layout = vLayout;
	}

}