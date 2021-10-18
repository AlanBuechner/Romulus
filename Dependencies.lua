VULKAN_SDK = os.getenv("VULKAN_SDK")

IncludeDir = {}
IncludeDir["spdlog"] = "%{wks.location}/Romulus/vendor/spdlog/include"
IncludeDir["ImGui"] = "%{wks.location}/Romulus/vendor/imgui"
IncludeDir["glm"] = "%{wks.location}/Romulus/vendor/glm"
IncludeDir["stb_image"] = "%{wks.location}/Romulus/vendor/stb_image"
IncludeDir["entt"] = "%{wks.location}/Romulus/vendor/entt/include"
IncludeDir["yaml"] = "%{wks.location}/Romulus/vendor/yaml-cpp/include"
IncludeDir["VulkanSDK"] = "%{VULKAN_SDK}/include"


LibraryDir = {}
LibraryDir["VulkanSDK"] = "%{VULKAN_SDK}/Lib"

Library = {}
Library["Vulkan"] = "%{LibraryDir.VulkanSDK}/vulkan-1.lib"
Library["VulkanUtils"] = "%{LibraryDir.VulkanSDK}/VkLayer_utils.lib"

Library["ShaderC"] = "%{LibraryDir.VulkanSDK}/shaderc_shared.lib"
Library["SPIRV_Cross"] = "%{LibraryDir.VulkanSDK}/spirv-cross-core.lib"
Library["SPIRV_Cross_GLSL"] = "%{LibraryDir.VulkanSDK}/spirv-cross-glsl.lib"