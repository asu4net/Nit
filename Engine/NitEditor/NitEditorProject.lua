project "NitEditor"
    kind "StaticLib"
    language "C++"
    cppdialect "C++20"
    targetdir (binariesdir)
    objdir (intermediatesdir)
    defines { "NIT_EDITOR" }
    pchheader "NitEditorPCH.h"
    pchsource "Source/NitEditorPCH.cpp"
    forceincludes { "NitEditorPCH.h" }

    files 
    { 
        "**.h", 
        "**.cpp" 
    }
    
    includedirs 
    { 
        "%{IncludeDirs.NitEditor}", 
        "%{IncludeDirs.Nit}", 
        "%{IncludeDirs.ImGui}", 
        "%{IncludeDirs.ImGuizmo}",
        "%{IncludeDirs.GLFW}",
        "%{IncludeDirs.entt}",
        "%{IncludeDirs.RTTR}"
    }
    
    links 
    { 
        "Nit", 
        "ImGui", 
        "ImGuizmo" 
    }

    filter "configurations:Debug"
        defines { "NIT_DEBUG" }
        symbols "On"

    filter "configurations:Release"
        defines { "NIT_RELEASE" }
        optimize "On"

    filter "system:windows"
        systemversion "latest"
        defines "NIT_WINDOWS"