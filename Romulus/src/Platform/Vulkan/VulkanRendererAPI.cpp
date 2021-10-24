#include "pch.h"
#include "VulkanRendererAPI.h"
#include <set>

// debug call back
static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
	VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData)
{

	switch (messageSeverity)
	{
	case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
	case VK_DEBUG_UTILS_MESSAGE_SEVERITY_FLAG_BITS_MAX_ENUM_EXT:
		return VK_FALSE;
	case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
		CORE_INFO("Validation Info: {0}", pCallbackData->pMessage);
		break;
		break;
	case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
		CORE_WARN("Validation Warn: {0}", pCallbackData->pMessage);
		break;
	case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
		CORE_ERROR("Validation Error: {0}", pCallbackData->pMessage);
		break;
	default:
		break;
	}

	return VK_FALSE;
}

static void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo)
{
	createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
	createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
	createInfo.pfnUserCallback = DebugCallback;
}

namespace Engine
{
	Engine::VulkanRendererAPI::VulkanRendererAPI()
	{
		m_ValidationLayers.push_back("VK_LAYER_KHRONOS_validation");

		m_Extentions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
		m_Extentions.push_back(VK_KHR_SURFACE_EXTENSION_NAME);
#if defined(PLATFORM_WINDOWS)
		m_Extentions.push_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);
#endif

		m_DeviceExtentions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);

		// init application info
		VkApplicationInfo appInfo{};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = "Hello Triangle";
		appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.pEngineName = "No Engine";
		appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.apiVersion = VK_API_VERSION_1_0;

		// init instance info
		VkInstanceCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pApplicationInfo = &appInfo;

		// enable extentions
		createInfo.enabledExtensionCount = static_cast<uint32>(m_Extentions.size());
		createInfo.ppEnabledExtensionNames = m_Extentions.data();

		// enable validation layers
#if defined(_DEBUG)
		VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
		createInfo.enabledLayerCount = static_cast<uint32>(m_ValidationLayers.size());
		createInfo.ppEnabledLayerNames = m_ValidationLayers.data();

		PopulateDebugMessengerCreateInfo(debugCreateInfo);
		createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
#else
		createInfo.enabledLayerCount = 0;
		createInfo.pNext = nullptr;
#endif // 0

		// create instance
		CORE_ASSERT(vkCreateInstance(&createInfo, nullptr, &m_Instance) == VK_SUCCESS, "Failed to create vulkan instance!");

		// get gpu
		m_PhysicalDevice = GetBestPhysicalDevice();
		CORE_ASSERT(m_PhysicalDevice != VK_NULL_HANDLE, "Could not get a sutable device!");

		vkGetPhysicalDeviceMemoryProperties(m_PhysicalDevice, &m_MemoryProps);

		// create device
		QueueFamilyIndices indices = GetDeviceQueueFamilyIndices(m_PhysicalDevice);

		VkDeviceQueueCreateInfo queueCreateInfo{};
		queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueCreateInfo.queueFamilyIndex = indices.graphicsFamily.value();
		queueCreateInfo.queueCount = 1;

		float queuePriority = 1.0f;
		queueCreateInfo.pQueuePriorities = &queuePriority;

		VkPhysicalDeviceFeatures deviceFeatures{};

		VkDeviceCreateInfo deviceCreateInfo{};
		deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

		deviceCreateInfo.pQueueCreateInfos = &queueCreateInfo;
		deviceCreateInfo.queueCreateInfoCount = 1;

		deviceCreateInfo.pEnabledFeatures = &deviceFeatures;

		deviceCreateInfo.enabledExtensionCount = 0;

#if defined(_DEBUG)
		deviceCreateInfo.enabledLayerCount = static_cast<uint32>(m_ValidationLayers.size());
		deviceCreateInfo.ppEnabledLayerNames = m_ValidationLayers.data();
		deviceCreateInfo.enabledExtensionCount = static_cast<uint32>(m_DeviceExtentions.size());
		deviceCreateInfo.ppEnabledExtensionNames = m_DeviceExtentions.data();
#else
		deviceCreateInfo.enabledLayerCount = 0;
#endif

		CORE_ASSERT(vkCreateDevice(m_PhysicalDevice, &deviceCreateInfo, nullptr, &m_Device) == VK_SUCCESS, "Failded to create device");

	}

	VulkanRendererAPI::~VulkanRendererAPI()
	{
		vkDestroyDevice(m_Device, nullptr);
		vkDestroyInstance(m_Instance, nullptr);
	}

	std::vector<VkPhysicalDevice> VulkanRendererAPI::GetPhysicalDevices()
	{
		// get number of devices
		uint32 deviceCount = 0;
		vkEnumeratePhysicalDevices(m_Instance, &deviceCount, nullptr);
		CORE_ASSERT(deviceCount != 0, "No devices with vulkan support found");

		// get the devices
		std::vector<VkPhysicalDevice> devices(deviceCount);
		vkEnumeratePhysicalDevices(m_Instance, &deviceCount, devices.data());

		return devices;
	}

	VkPhysicalDevice VulkanRendererAPI::GetBestPhysicalDevice()
	{
		std::vector<VkPhysicalDevice> devices = GetPhysicalDevices();
		VkPhysicalDevice gpu = VK_NULL_HANDLE;

		// enumerate over all devices and check if it is sutibal
		for (const auto& device : devices) 
		{
			if (GetDeviceQueueFamilyIndices(device).isComplete() && checkDeviceExtensionSupport(device)) 
			{
				gpu = device;
				break;
			}
		}

		return gpu;
	}

	bool VulkanRendererAPI::checkDeviceExtensionSupport(VkPhysicalDevice device) {
		uint32 extensionCount;
		vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

		std::vector<VkExtensionProperties> availableExtensions(extensionCount);
		vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

		std::set<std::string> requiredExtensions(m_DeviceExtentions.begin(), m_DeviceExtentions.end());

		for (const auto& extension : availableExtensions) {
			requiredExtensions.erase(extension.extensionName);
		}

		return requiredExtensions.empty();
	}

	VulkanRendererAPI::QueueFamilyIndices VulkanRendererAPI::GetDeviceQueueFamilyIndices(VkPhysicalDevice device)
	{
		QueueFamilyIndices indices;

		uint32 queueFamilyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

		std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

		int i = 0;
		for (const auto& queueFamily : queueFamilies) 
		{
			if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
				indices.graphicsFamily = i;

			if (indices.isComplete())
				break;

			i++;
		}

		return indices;
	}

}
