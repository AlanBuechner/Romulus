#pragma once
#include "Engine\Core\Core.h"
#include "Engine\Renderer\RendererAPI.h"

#if defined(PLATFORM_WINDOWS)
#define VK_USE_PLATFORM_WIN32_KHR
#endif

#define VK_PROTOTYPES
#include <vulkan\vulkan.h>

#include <vector>

namespace Engine
{
	class VulkanRendererAPI : public RendererAPI
	{
		struct QueueFamilyIndices
		{
			std::optional<uint32_t> graphicsFamily;

			bool isComplete() { return graphicsFamily.has_value(); }
		};

	public:
		VulkanRendererAPI();
		~VulkanRendererAPI();

	private:
		std::vector<VkPhysicalDevice> GetPhysicalDevices();
		VkPhysicalDevice GetBestPhysicalDevice();

		QueueFamilyIndices GetDeviceQueueFamilyIndices(VkPhysicalDevice device);

	private:
		VkInstance m_Instance = VK_NULL_HANDLE;
		VkPhysicalDevice m_PhysicalDevice = VK_NULL_HANDLE;
		VkDevice m_Device = VK_NULL_HANDLE;

		std::vector<const char*> m_ValidationLayers;
		std::vector<const char*> m_Extentions;
	};
}

