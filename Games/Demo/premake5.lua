IncludeDirs["DemoSource"] = "%{wks.location}/Games/Demo/DemoSource/Source"

group "Games/Demo"
    include "DemoEditor/premake5.lua"
    include "DemoSource/premake5.lua"
    include "Demo/premake5.lua"
group ""