#pragma once
#include "Engine\Core\Core.h"
#include "Engine\Renderer\RendererAPI.h"

#define VK_PROTOTYPES
#include <vulkan\vulkan.h>

#include <vector>

namespace Engine
{
	class VulkanRendererAPI : public RendererAPI
	{
		struct QueueFamilyIndices
		{
			std::optional<uint32> graphicsFamily;

			bool isComplete() { return graphicsFamily.has_value(); }
		};

	public:
		VulkanRendererAPI();
		~VulkanRendererAPI();

		VkInstance GetInstance() { return m_Instance; }
		VkPhysicalDevice GetPhysicalDevice() { return m_PhysicalDevice; }
		VkPhysicalDeviceMemoryProperties GetMemoryProps() { return m_MemoryProps; }
		QueueFamilyIndices GetDeviceQueueFamilyIndices(VkPhysicalDevice device);
		VkDevice GetDevice() { return m_Device; }

	private:
		std::vector<VkPhysicalDevice> GetPhysicalDevices();
		VkPhysicalDevice GetBestPhysicalDevice();
		bool checkDeviceExtensionSupport(VkPhysicalDevice device);


	private:
		VkInstance m_Instance = VK_NULL_HANDLE;
		VkPhysicalDevice m_PhysicalDevice = VK_NULL_HANDLE;
		VkDevice m_Device = VK_NULL_HANDLE;

		VkPhysicalDeviceMemoryProperties m_MemoryProps;

		std::vector<const char*> m_ValidationLayers;
		std::vector<const char*> m_Extentions;
		std::vector<const char*> m_DeviceExtentions;
	};
}

