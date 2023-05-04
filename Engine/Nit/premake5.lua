project "Nit"
    kind "StaticLib"
    language "C++"
    cppdialect "C++20"
    staticruntime "off"
    
    outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

    targetdir ("%{wks.location}/Binaries/" .. outputdir .. "/%{prj.name}")
    objdir ("%{wks.location}/Intermediate/" .. outputdir .. "/%{prj.name}")

    pchheader "NitPCH.h"
    pchsource "Source/NitPCH.cpp"

    forceincludes { "NitPCH.h" }

    --openal
    disablewarnings 
	{ 
		"5030"
	}

    files
    {
        "%{prj.location}/Source/**.h",
        "%{prj.location}/Source/**.cpp"
    }

    includedirs
    {
        "%{prj.location}/Source",
        "%{IncludeDirs.glad}",
        "%{IncludeDirs.glfw}",
        "%{IncludeDirs.glm}",
        "%{IncludeDirs.stb}",
        "%{IncludeDirs.rttr}",
        "%{IncludeDirs.rapidjson}",
        "%{IncludeDirs.openal}",
        "%{IncludeDirs.openalSource}",
        "%{IncludeDirs.openalCommon}"
    }

    links
    {
        "opengl32.lib",
        "glad",
        "glfw",
        "rttr",
        "openal"
    }

    defines 
    {
        "_CRT_SECURE_NO_WARNINGS",
        "_SILENCE_CXX23_ALIGNED_STORAGE_DEPRECATION_WARNING",
        "AL_LIBTYPE_STATIC"
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

        links
        {
            "imgui",
            "imguizmo"
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

        links
        {
            "imgui",
            "imguizmo"
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

        linkoptions { "-IGNORE:4006" }