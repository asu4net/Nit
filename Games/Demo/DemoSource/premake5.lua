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
        "%{prj.location}/Content/**.glsl"
    }

    includedirs
    {
        "%{prj.location}/Source",
        "%{IncludeDirs.Nit}",
        "%{IncludeDirs.glm}",
        "%{IncludeDirs.rttr}"
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
		("{COPY} ../%{file.relpath}/Content %{wks.location}/Binaries/" .. outputdir .. "/Demo/Content")
	}

    filter "system:windows"
        systemversion "latest"
    
    filter "configurations:Debug"
        defines "NIT_DEBUG"
        runtime "Debug"
        symbols "on"
    
    filter "configurations:Release"
        defines "NIT_RELEASE"
        runtime "Release"
        optimize "on"