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
        "%{IncludeDirs.yaml}",
        "%{IncludeDirs.rttr}"
    }

    links
    {
        "opengl32.lib",
        "glad",
        "glfw",
        "yaml-cpp",
        "rttr"
    }

    defines 
    {
        "_SILENCE_CXX23_ALIGNED_STORAGE_DEPRECATION_WARNING"
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