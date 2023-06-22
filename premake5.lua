include "./vendor/premake/premake_customization/solution_items.lua"

workspace "Flood"
    architecture "x86_64"
    startproject "FloodEditor"

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

IncludeDir = {}
IncludeDir["GLFW"] = "%{wks.location}/Flood/vendor/GLFW/include"
IncludeDir["Glad"] = "%{wks.location}/Flood/vendor/Glad/include"
IncludeDir["ImGui"] = "%{wks.location}/Flood/vendor/imgui"
IncludeDir["glm"] = "%{wks.location}/Flood/vendor/glm"
IncludeDir["stb_image"] = "%{wks.location}/Flood/vendor/stb_image"
IncludeDir["yaml_cpp"] = "%{wks.location}/Flood/vendor/yaml-cpp/include"
IncludeDir["ImGuizmo"] = "%{wks.location}/Flood/vendor/ImGuizmo"

group "Dependencies"
	include "vendor/premake"
	include "Flood/vendor/GLFW"
	include "Flood/vendor/Glad"
	include "Flood/vendor/imgui"
	include "Flood/vendor/yaml-cpp"
group ""

include "Flood"
include "ExampleApp"
include "FloodEditor"