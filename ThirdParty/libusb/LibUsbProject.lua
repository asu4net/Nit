project "LibUsb"
    kind "StaticLib"
    language "C"
    staticruntime "off"
    targetdir (binariesdir)
    objdir (intermediatesdir)

    files
    {
        "src/*.h",
        "src/*.c",
        "include/*.h"
    }
    
    includedirs
    {
        "include",
        "${WINHID_INCLUDE_DIRS}"
    }
    
    defines 
    {
        "_CRT_SECURE_NO_WARNINGS",
        "_UNICODE",
        "UNICODE"
    }

    filter "configurations:Debug"
        defines 
        {
            "NIT_DEBUG"
        }
        symbols "On"

    filter "configurations:Release"
        defines { "NIT_RELEASE" }
        optimize "On"

    filter "system:windows"
        systemversion "latest"
        defines 
        { 
            "_WIN32_WINNT=_WIN32_WINNT_VISTA",
        }
        files
        {
            "src/os/windows/**.h",
            "src/os/windows/**.cpp",
        }