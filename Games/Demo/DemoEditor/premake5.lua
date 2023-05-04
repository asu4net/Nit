project "DemoEditor"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"
    staticruntime "off"
    
    outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

    targetdir ("%{wks.location}/Binaries/" .. outputdir .. "/Demo")
    objdir ("%{wks.location}/Intermediate/" .. outputdir .. "/Demo")

    pchheader "DemoEditorPCH.h"
    pchsource "Source/DemoEditorPCH.cpp"

    forceincludes { "DemoEditorPCH.h" }

    files
    {
        "%{prj.location}/Source/**.h",
        "%{prj.location}/Source/**.cpp"
    }

    includedirs
    {
        "%{prj.location}/Source",
        "%{IncludeDirs.Nit}",
        "%{IncludeDirs.NitEditor}",
        "%{IncludeDirs.DemoSource}",
        "%{IncludeDirs.imgui}",
        "%{IncludeDirs.imguizmo}",
        "%{IncludeDirs.glm}",
        "%{IncludeDirs.rttr}"
    }

    links
    {
        "Nit",
        "NitEditor",
        "DemoSource"
    }

    defines 
    {
        "_SILENCE_CXX23_ALIGNED_STORAGE_DEPRECATION_WARNING",
        "NIT_IMGUI"
    }

    filter "system:windows"
        systemversion "latest"
    
    filter "configurations:Debug"
        defines "NIT_DEBUG"
        runtime "Debug"
        symbols "on"
    
    filter "configurations:EditorRelease"
        defines "NIT_RELEASE"
        runtime "Release"
        optimize "on"