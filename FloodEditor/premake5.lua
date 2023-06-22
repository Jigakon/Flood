project "FloodEditor"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

    targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
    objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "src/**.h",
        "src/**.cpp",
    }
    
    includedirs
    {
        "%{wks.location}/Flood/vendor/spdlog/include",
        "%{wks.location}/Flood/src",
        "%{wks.location}/Flood/vendor",
		"%{IncludeDir.glm}",
		"%{IncludeDir.ImGuizmo}",
    }

    links
	{
		"Flood"
	}

    filter "system:windows"
        systemversion "latest"
    
    filter "configurations:Debug"
        defines "FLOOD_DEBUG"
        runtime "Debug"
        symbols "On"
    
    filter "configurations:Release"
        defines "FLOOD_RELEASE"
        runtime "Release"
        optimize "On"

    filter "configurations:Dist"
        defines "FLOOD_DIST"
        runtime "Release"
        optimize "On"