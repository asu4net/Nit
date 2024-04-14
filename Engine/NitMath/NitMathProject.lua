project "NitMath"
    kind "StaticLib"
    language "C++"
    cppdialect "C++20"
    targetdir (binariesdir)
    objdir (intermediatesdir)
    defines { "NIT_ENGINE" }
    pchheader "NitMathPCH.h"
    pchsource "NitMath/NitMathPCH.cpp"
    forceincludes { "NitMathPCH.h" }
    
    includedirs 
    {
        "%{IncludeDirs.NitMath}"
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