project "NitEditor"
    kind "StaticLib"
    language "C++"
    cppdialect "C++20"
    staticruntime "off"
    
    outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

    targetdir ("%{wks.location}/Binaries/" .. outputdir .. "/%{prj.name}")
    objdir ("%{wks.location}/Intermediate/" .. outputdir .. "/%{prj.name}")

    pchheader "NitEditorPCH.h"
    pchsource "Source/NitEditorPCH.cpp"

    forceincludes { "NitEditorPCH.h" }

    files
    {
        "%{prj.location}/Source/**.h",
        "%{prj.location}/Source/**.cpp"
    }

    includedirs
    {
        "%{prj.location}/Source",
        "%{IncludeDirs.Nit}",
        "%{IncludeDirs.glfw}",
        "%{IncludeDirs.imgui}",
        "%{IncludeDirs.imguizmo}",
        "%{IncludeDirs.glm}",
        "%{IncludeDirs.rttr}"
    }

    libdirs
    {
    }

    links
    {
        "Nit",
        "imgui",
        "imguizmo"
    }

    defines 
    {
        "_CRT_SECURE_NO_WARNINGS",
        "_SILENCE_CXX23_ALIGNED_STORAGE_DEPRECATION_WARNING",
        "NIT_EDITOR"
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