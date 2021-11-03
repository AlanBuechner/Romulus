project "Romulus"
	kind "StaticLib"
	language "C++"
	cppdialect "C++20"
	staticruntime "on"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "pch.h"
	pchsource "src/pch.cpp"

	files
	{
		"src/**.h",
		"src/**.cpp",
		"vendor/glm/glm/**.hpp",
		"vendor/glm/glm/**.inl",
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS"
	}

	includedirs
	{
		"src",
		"%{IncludeDir.spdlog}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb_image}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.yaml}",
		"%{IncludeDir.VulkanSDK}",
	}

	links 
	{ 
		"ImGui",
		"Yaml",
		"%{Library.Vulkan}",
		"%{Library.ShaderC}",
		"%{Library.SPIRV_Cross}",
		"%{Library.SPIRV_Cross_GLSL}",
	}

	filter "files:vendor/stb_image/**.cpp"
		flags { "NoPCH" }

	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		defines "DEBUG"
		buildoptions "/MTd"
		runtime "Debug"
		symbols "on"

		defines
		{
			DEBUG
		}

	filter "configurations:Release"
		defines "RELEASE"
		buildoptions "/MT"
		runtime "Release"
		optimize "on"

		defines
		{
			RELEASE
		}

	filter "configurations:Dist"
		defines "DIST"
		buildoptions "/MT"
		runtime "Release"
		optimize "on"

		defines
		{
			DIST
		}
