workspace "Nit"
    architecture "x86_64"
    startproject "DemoEditor"

    configurations
    {
        "Debug",
        "GameRelease",
        "EditorRelease"
    }

include "Directories.lua"

group "ThirdParty"
    include "ThirdParty/glad/premake5.lua"  
    include "ThirdParty/glfw/premake5.lua"  
    include "ThirdParty/glm/premake5.lua"
    include "ThirdParty/imgui/premake5.lua"
    include "ThirdParty/imguizmo/premake5.lua"
    include "ThirdParty/stb/premake5.lua"
    include "ThirdParty/rttr/premake5.lua"
    include "ThirdParty/rapidjson/premake5.lua"
    include "ThirdParty/openal/premake5.lua"
group ""

group "Engine"
    include "Engine/Nit/premake5.lua"
    include "Engine/NitEditor/premake5.lua"
group ""

include "Game/premake5.lua"