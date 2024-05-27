project "JoyShockLibrary"
    kind "StaticLib"
    language "C++"
    cppdialect "C++20"
    staticruntime "off"
    targetdir (binariesdir)
    objdir (intermediatesdir)

    files
    {
        "src/**.h",
        "src/**.cpp",
        "include/**.h"
    }
    
    includedirs
    {
        "include",
        "$(ProjectDir)src/hidapi"
    }

    defines
    {
        "GYROCONTROLLERLIBRARY_EXPORTS",
        "_USRDLL"
    }

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
            "_WINDOWS"
        }