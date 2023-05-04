project "GameApp"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"
    staticruntime "off"
    
    outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

    targetdir ("%{wks.location}/Binaries/" .. outputdir .. "/Game")
    objdir ("%{wks.location}/Intermediate/" .. outputdir .. "/Game")

    pchheader "GameAppPCH.h"
    pchsource "Source/GameAppPCH.cpp"

    forceincludes { "GameAppPCH.h" }

    files
    {
        "%{prj.location}/Source/**.h",
        "%{prj.location}/Source/**.cpp",
    }

    includedirs
    {
        "%{prj.location}/Source",
        "%{IncludeDirs.Nit}",
        "%{IncludeDirs.Game}",
        "%{IncludeDirs.glm}",
        "%{IncludeDirs.rttr}",
    }

    links
    {
        "Nit",
        "Game"
    }

    defines 
    {
        "_SILENCE_CXX23_ALIGNED_STORAGE_DEPRECATION_WARNING"
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

    filter "configurations:GameRelease"
        defines "NIT_RELEASE"
        runtime "Release"
        optimize "on"