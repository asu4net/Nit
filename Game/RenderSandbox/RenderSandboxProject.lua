project "RenderSandboxProject"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"
    targetdir (binariesdir)
    objdir (intermediatesdir)
    debugdir (workingdir)
    defines { "NIT_GAME" }

    includedirs
    { 
        "%{prj.dir}",
        "%{IncludeDirs.NitEngine}"
    }
    
    links 
    { 
        "NitRender"
    }
    
    files { "**.h", "**.cpp" }
    
    filter "configurations:Debug"
        defines { "NIT_DEBUG" }
        symbols "On"

    filter "configurations:Release"
        kind "WindowedApp"
        entrypoint "mainCRTStartup"
        defines { "NIT_RELEASE" }
        optimize "On"

    filter "system:windows"
        systemversion "latest"
        defines "NIT_WINDOWS"