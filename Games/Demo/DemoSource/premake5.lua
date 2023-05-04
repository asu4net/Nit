project "DemoSource"
    kind "StaticLib"
    language "C++"
    cppdialect "C++20"
    staticruntime "off"
    
    outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

    targetdir ("%{wks.location}/Binaries/" .. outputdir .. "/Demo")
    objdir ("%{wks.location}/Intermediate/" .. outputdir .. "/Demo")

    pchheader "DemoSourcePCH.h"
    pchsource "Source/DemoSourcePCH.cpp"

    forceincludes { "DemoSourcePCH.h" }

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
        "%{IncludeDirs.imguizmo}"
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
		("{COPY} %{wks.location}/Games/Demo/Content %{wks.location}/Binaries/" .. outputdir .. "/Demo/Content")
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