project "stb_image"
    kind "StaticLib"
    language "C++"
    cppdialect "C++20"
    staticruntime "On"
    
    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
    
    files
	{
		"stb_image.h",
		"stb_image.cpp",
	}
    
    includedirs
    {
        "$(ProjectDir)"
	}

    filter "system:windows"
        systemversion "latest"
        staticruntime "On"
    
    filter "configurations:Debug"
    	runtime "Debug"
    	symbols "on"
    
    filter "configurations:Release"
    	runtime "Release"
    	optimize "on"