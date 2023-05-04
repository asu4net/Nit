IncludeDirs["Game"] = "%{wks.location}/Game/Game/Source"

group "Game"
    include "GameEditorApp/premake5.lua"
    include "Game/premake5.lua"
    include "GameApp/premake5.lua"
group ""