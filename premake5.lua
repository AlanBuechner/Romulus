include "./vendor/premake/premake_customization/solution_items.lua"
include "Dependencies.lua"

workspace "Romulus"
	architecture "x64"
	startproject "Tritom"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

	solution_items
	{
		".editorconfig"
	}

	flags
	{
		"MultiProcessorCompile"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

group "Dependencies"
	include "Romulus/vendor/imgui"
	include "Romulus/vendor/yaml-cpp"
	include "Romulus/vendor/stb_image"
group ""


include "Romulus" -- engine
include "Remus" -- editor
include "Tritom" -- runtime
