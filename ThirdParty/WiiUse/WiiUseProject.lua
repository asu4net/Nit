project "WiiUse"
    kind "StaticLib"
    language "C"
    staticruntime "off"
    targetdir (binariesdir)
    objdir (intermediatesdir)

    files
    {
        "src/**.h",
        "src/**.c",
        "include/**.h"
    }
    
    removefiles 
    { 
        "src/os_mac/**.c",
        "src/os_mac/**.h",
        "src/os_mac/**.m" 
    }

    includedirs
    {
        "include",
        "${WINHID_INCLUDE_DIRS}"
    }

    filter "configurations:Debug"
        defines { "NIT_DEBUG" }
        symbols "On"

    filter "configurations:Release"
        defines { "NIT_RELEASE" }
        optimize "On"

    filter "system:windows"
        systemversion "latest"
        defines 
        { 
            "NIT_WINDOWS",
            "WIN32",
            "_WINDOWS",
            "WIIUSE_STATIC",
            "_WIN32_WINNT=0x0501",
            "WIIUSE_COMPILE_LIB"
         }