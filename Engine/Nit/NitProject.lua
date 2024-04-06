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
    
    disablewarnings 
	{ 
		"5030",
		"4065",
		"4834"
	}

    includedirs 
    { 
        "%{IncludeDirs.NitMath}",
        "%{IncludeDirs.Nit}",
        "%{IncludeDirs.GLFW}",
        "%{IncludeDirs.GLAD}",
        "%{IncludeDirs.STB}",
        "%{IncludeDirs.entt}",
        "%{IncludeDirs.RTTR}",
        "%{IncludeDirs.rapidjson}",
        "%{IncludeDirs.OpenALInclude}",
        "%{IncludeDirs.OpenALSource}",
        "%{IncludeDirs.OpenALCommon}",
        "%{IncludeDirs.Box2D}",
    }
    
    links
    {
        "NitMath",
        "GLFW",
        "opengl32.lib",
        "GLAD",
        "RTTR",
        "OpenAL",
        "Box2D"
    }

    defines
    {
        "_CRT_SECURE_NO_WARNINGS",
        "_SILENCE_CXX23_ALIGNED_STORAGE_DEPRECATION_WARNING",
        "AL_LIBTYPE_STATIC"
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