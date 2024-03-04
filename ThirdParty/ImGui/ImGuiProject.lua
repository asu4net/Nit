project "ImGui"
	kind "StaticLib"
    language "C++"
    cppdialect "C++20"
    staticruntime "off"
	targetdir (binariesdir)
    objdir (intermediatesdir)

	files
	{
		"Source/*.h",
		"Source/*.cpp"
	}
	
	includedirs
	{
		"%{IncludeDirs.ImGui}"
	}

	filter "system:windows"
    systemversion "latest"
    defines "NIT_WINDOWS"

    filter "configurations:Debug"
        defines { "NIT_DEBUG" }
        symbols "On"

    filter "configurations:Release"
        defines { "NIT_RELEASE" }
        optimize "On"
