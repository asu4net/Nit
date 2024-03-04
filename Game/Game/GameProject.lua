project "Game"
    kind "StaticLib"
    language "C++"
    cppdialect "C++20"
    targetdir (binariesdir)
    objdir (intermediatesdir)
    defines { "NIT_GAME" }
    pchheader "GamePCH.h"
    pchsource "Source/GamePCH.cpp"
    forceincludes { "GamePCH.h" }

    includedirs 
    { 
        "%{IncludeDirs.Game}", 
        "%{IncludeDirs.Nit}",
        "%{IncludeDirs.entt}",
        "%{IncludeDirs.RTTR}" 
    }
    
    links { "Nit" }
    
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