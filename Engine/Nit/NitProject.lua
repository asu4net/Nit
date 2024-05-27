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
        "%{IncludeDirs.WiiUse}",
        --"%{IncludeDirs.JoyShockLibrary}"
    }
    
    links
    {
        "GLFW",
        "opengl32.lib",
        "GLAD",
        "RTTR",
        "OpenAL",
        "Box2D",
        "hid.lib",
        "setupapi.lib",
        "WiiUse",
        --"JoyShockLibrary"
    }

    defines
    {
        "_CRT_SECURE_NO_WARNINGS",
        "_SILENCE_CXX23_ALIGNED_STORAGE_DEPRECATION_WARNING",
        "AL_LIBTYPE_STATIC",
        "WIIUSE_STATIC",
        "WIIUSE_COMPILE_LIB",
        --"GYROCONTROLLERLIBRARY_EXPORTS",
        --"_USRDLL"
    }

    files { "**.h", "**.cpp" }

    filter "configurations:Debug"
        defines 
        { 
            "NIT_DEBUG",
            "_DEBUG"
        }
        symbols "On"

    filter "configurations:Release"
        defines 
        {
            "NIT_RELEASE",
            "NDEBUG"
        }
        optimize "On"

    filter "system:windows"
        systemversion "latest"
        defines 
        {   
            "NIT_WINDOWS",
            "WIN32",
            "_WINDOWS",
            "_WIN32_WINNT=0x0501",
            --"_WIN32_WINNT=_WIN32_WINNT_VISTA",
        }

        links
        {        
            "ws2_32.lib",
        }