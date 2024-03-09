project "GameApp"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"
    targetdir (binariesdir)
    objdir (intermediatesdir)
    debugdir (workingdir)
    defines { "NIT_GAME" }
    pchheader "GameAppPCH.h"
    pchsource "Source/GameAppPCH.cpp"
    forceincludes { "GameAppPCH.h" }

    includedirs
    { 
        "%{IncludeDirs.GameApp}", 
        "%{IncludeDirs.Nit}", 
        "%{IncludeDirs.Game}",
        "%{IncludeDirs.entt}",
        "%{IncludeDirs.RTTR}"
    }
    
    links { "Game" }
    
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