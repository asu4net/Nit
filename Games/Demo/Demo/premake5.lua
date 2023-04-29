project "Demo"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"
    staticruntime "off"
    
    outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

    targetdir ("%{wks.location}/Binaries/" .. outputdir .. "/Demo")
    objdir ("%{wks.location}/Intermediate/" .. outputdir .. "/Demo")

    pchheader "DemoPCH.h"
    pchsource "Source/DemoPCH.cpp"

    forceincludes { "DemoPCH.h" }

    files
    {
        "%{prj.location}/Source/**.h",
        "%{prj.location}/Source/**.cpp",
    }

    includedirs
    {
        "%{prj.location}/Source",
        "%{IncludeDirs.Nit}",
        "%{IncludeDirs.DemoSource}",
        "%{IncludeDirs.glm}",
        "%{IncludeDirs.rttr}"
    }

    links
    {
        "Nit",
        "DemoSource"
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