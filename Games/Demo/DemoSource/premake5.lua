project "DemoSource"
    kind "StaticLib"
    language "C++"
    cppdialect "C++20"
    staticruntime "on"
    
    outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

    targetdir ("%{wks.location}/Binaries/" .. outputdir .. "/%{prj.name}")
    objdir ("%{wks.location}/Intermediate/" .. outputdir .. "/%{prj.name}")

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
        "%{IncludeDirs.glm}"
    }

    links
    {
        "Nit"
    }

    defines 
    {
    }

    postbuildcommands
	{
		("{COPY} ../%{file.relpath}/Content %{wks.location}/Binaries/" .. outputdir .. "/Demo/Content"),
		("{COPY} ../%{file.relpath}/Content %{wks.location}/Binaries/" .. outputdir .. "/DemoEditor/Content")
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