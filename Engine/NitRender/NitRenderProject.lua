project "NitRender"
    kind "StaticLib"
    language "C++"
    cppdialect "C++20"
    targetdir (binariesdir)
    objdir (intermediatesdir)
    defines { "NIT_ENGINE" }
    pchheader "NitRenderPCH.h"
    pchsource "NitRenderPCH.cpp"
    forceincludes { "NitRenderPCH.h" }
    
    includedirs 
    {
        "%{prj.dir}", 
        "%{IncludeDirs.NitEngine}",
        "%{IncludeDirs.GLAD}",
        "%{IncludeDirs.GLFW}",
        "%{IncludeDirs.STB}"
    }

    links
    {
        "NitMath",
        "opengl32.lib",
        "GLAD",
        "GLFW",
    }

    files { "**.h", "**.cpp" }

    filter "configurations:Debug"
        defines { "NIT_DEBUG" }
        symbols "On"

    filter "configurations:Release"
        defines { "NIT_RELEASE" }
        optimize "On"

    filter "system:windows"
        systemversion "latest"
        defines "NIT_WINDOWS"