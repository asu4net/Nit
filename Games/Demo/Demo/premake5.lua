project "Demo"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"
    staticruntime "on"
    
    outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

    targetdir ("%{wks.location}/Binaries/" .. outputdir .. "/%{prj.name}")
    objdir ("%{wks.location}/Intermediate/" .. outputdir .. "/%{prj.name}")

    debugdir = "%{wks.location}/Games/Demo/DemoSource"

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
        "%{IncludeDirs.glm}"
    }

    links
    {
        "Nit",
        "DemoSource"
    }

    defines 
    {
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