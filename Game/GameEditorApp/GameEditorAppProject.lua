project "GameEditorApp"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"
    targetdir (binariesdir)
    objdir (intermediatesdir)
    debugdir (workingdir)
    defines { "NIT_GAME_EDITOR" }
    pchheader "GameEditorAppPCH.h"
    pchsource "Source/GameEditorAppPCH.cpp"
    forceincludes { "GameEditorAppPCH.h" }

    includedirs 
    { 
        "%{IncludeDirs.GameEditor}", 
        "%{IncludeDirs.Nit}", 
        "%{IncludeDirs.NitEditor}", 
        "%{IncludeDirs.Game}",
        "%{IncludeDirs.entt}",
        "%{IncludeDirs.RTTR}" 
    }
    
    links { "NitEditor", "Game" }
    
    files { "**.h", "**.cpp" }

    filter "configurations:Debug"
        defines { "NIT_DEBUG" }
        symbols "On"

    filter "configurations:Release"
        kind "WindowedApp"
        entrypoint "mainCRTStartup"
        defines { "NIT_RELEASE" }
        optimize "On"

    filter "system:windows"
        systemversion "latest"
        defines "NIT_WINDOWS"