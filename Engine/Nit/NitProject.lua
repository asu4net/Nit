project "Nit"
    kind "StaticLib"
    language "C++"
    cppdialect "C++20"
    targetdir (binariesdir)
    objdir (intermediatesdir)
    defines { "NIT_ENGINE" }
    pchheader "NitPCH.h"
    pchsource "Source/NitPCH.cpp"
    forceincludes { "NitPCH.h" }
    
    includedirs 
    { 
        "%{IncludeDirs.Nit}",
        "%{IncludeDirs.GLFW}",
        "%{IncludeDirs.GLAD}",
        "%{IncludeDirs.STB}",
        "%{IncludeDirs.entt}",
        "%{IncludeDirs.RTTR}",
        "%{IncludeDirs.rapidjson}" 
    }
    
    links
    {
        "GLFW",
        "opengl32.lib",
        "GLAD",
        "RTTR"
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