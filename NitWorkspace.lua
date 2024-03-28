workspace "Nit"
    architecture "x86_64"
    configurations { "Debug", "Release" }
    startproject "GameEditorApp"

include "NitDirectories.lua"

group "Engine"
    include "Engine/Nit/NitProject.lua"
    include "Engine/NitEditor/NitEditorProject.lua"
group ""

group "Game"
    include "Game/Game/GameProject.lua"
    include "Game/GameEditorApp/GameEditorAppProject.lua"
    include "Game/GameApp/GameAppProject.lua"
group ""

group "ThirdParty"
    include "ThirdParty/GLFW/GLFWProject.lua"
    include "ThirdParty/GLAD/GLADProject.lua"
    include "ThirdParty/RTTR/RTTRProject.lua"
    include "ThirdParty/ImGui/ImGuiProject.lua"
    include "ThirdParty/ImGuizmo/ImGuizmoProject.lua"
    include "ThirdParty/OpenAL/OpenALProject.lua"
group ""