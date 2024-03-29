outputdir                      = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
binariesdir                    = "%{wks.location}/Binaries/"              .. outputdir .. "/%{prj.name}"
intermediatesdir               = "%{wks.location}/Binaries-Intermediate/" .. outputdir .. "/%{prj.name}"
workingdir                     = "..\\Assets"

IncludeDirs                    = {}

IncludeDirs["Nit"]             = "%{wks.location}/Engine/Nit/Source"
IncludeDirs["NitEditor"]       = "%{wks.location}/Engine/NitEditor/Source"
IncludeDirs["Game"]            = "%{wks.location}/Game/Game/Source"
IncludeDirs["GameApp"]         = "%{wks.location}/Game/GameApp/Source"
IncludeDirs["GameEditorApp"]   = "%{wks.location}/Game/GameEditorApp/Source"

IncludeDirs["GLFW"]            = "%{wks.location}/ThirdParty/GLFW/include"
IncludeDirs["GLAD"]            = "%{wks.location}/ThirdParty/GLAD/include"
IncludeDirs["STB"]             = "%{wks.location}/ThirdParty/STB/include"
IncludeDirs["entt"]            = "%{wks.location}/ThirdParty/entt/include"
IncludeDirs["RTTR"]            = "%{wks.location}/ThirdParty/rttr/src"
IncludeDirs["rapidjson"]       = "%{wks.location}/ThirdParty/rapidjson/include"
IncludeDirs["ImGui"]           = "%{wks.location}/ThirdParty/ImGui/Source"
IncludeDirs["ImGuizmo"]        = "%{wks.location}/ThirdParty/ImGuizmo/Source"
IncludeDirs["OpenALInclude"]   = "%{wks.location}/ThirdParty/OpenAL/include"
IncludeDirs["OpenALSource"]    = "%{wks.location}/ThirdParty/OpenAL/src"
IncludeDirs["OpenALCommon"]    = "%{wks.location}/ThirdParty/OpenAL/src/common"