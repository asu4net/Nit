project "Game"
    kind "StaticLib"
    language "C++"
    cppdialect "C++20"
    staticruntime "off"
    
    outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

    targetdir ("%{wks.location}/Binaries/" .. outputdir .. "/Game")
    objdir ("%{wks.location}/Intermediate/" .. outputdir .. "/Game")

    pchheader "GamePCH.h"
    pchsource "Source/GamePCH.cpp"

    forceincludes { "GamePCH.h" }

    files
    {
        "%{prj.location}/Source/**.h",
        "%{prj.location}/Source/**.cpp",
        "%{prj.location}/../Content/**.glsl"
    }

    includedirs
    {
        "%{prj.location}/Source",
        "%{IncludeDirs.Nit}",
        "%{IncludeDirs.glm}",
        "%{IncludeDirs.rttr}",
        "%{IncludeDirs.imgui}",
        "%{IncludeDirs.imguizmo}",
        "%{IncludeDirs.entt}"
    }

    links
    {
        "Nit"
    }

    defines 
    {
        "_SILENCE_CXX23_ALIGNED_STORAGE_DEPRECATION_WARNING"
    }

    postbuildcommands
	{
		("{COPY} %{wks.location}/Game/Content %{wks.location}/Binaries/" .. outputdir .. "/Game/Content")
	}

    filter "system:windows"
        systemversion "latest"
    
    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"

        includedirs
        {
            "%{IncludeDirs.imgui}",
            "%{IncludeDirs.imguizmo}"
        }

        defines
        {
            "NIT_DEBUG",
            "NIT_IMGUI"
        }

    filter "configurations:EditorRelease"
        runtime "Release"
        optimize "on"

        includedirs
        {
            "%{IncludeDirs.imgui}",
            "%{IncludeDirs.imguizmo}"
        }

        defines
        {
            "NIT_RELEASE",
            "NIT_IMGUI"
        }

    filter "configurations:GameRelease"
        defines "NIT_RELEASE"
        runtime "Release"
        optimize "on"