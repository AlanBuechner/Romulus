#pragma once
#include "Engine\Renderer\Texture.h"

#include <vulkan\vulkan.h>

namespace Engine
{
	class VulkanTexture : public Texture
	{
	public:
		VulkanTexture() = default;
		VulkanTexture(uint32 width, uint32 height, TextureFormat format, void* data);
		~VulkanTexture();

		inline VkImage GetVulkanImage() const { return m_ImageBuffer; }
		inline VkFormat GetVulkanFormat() const { return m_ImageFormat; }
		inline VkImageLayout GetVulkanLayout() const { return m_Layout; }
		inline VkImageView GetView() const { return m_ImageView; }

		inline void SetVulkanLayout(VkImageLayout layout) { m_Layout = layout; }

		void CreateImage(uint32 width, uint32 height, VkImage image, VkImageView view, VkFormat vFormat, VkDeviceMemory memory, VkImageLayout vLayout, TextureFormat format);

	private:

		VkImage m_ImageBuffer = VK_NULL_HANDLE;
		VkImageView m_ImageView = VK_NULL_HANDLE;
		VkFormat m_ImageFormat = VK_FORMAT_UNDEFINED;
		VkDeviceMemory m_ImageMemory;

		VkImageLayout m_Layout = VK_IMAGE_LAYOUT_UNDEFINED;

	};
}