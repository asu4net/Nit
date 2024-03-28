project "OpenAL"
	kind "StaticLib"
	language "C++"
	cppdialect "C++20"
	staticruntime "off"
	targetdir (binariesdir)
	objdir (intermediatesdir)

	disablewarnings 
	{ 
		"5030",
		"4065",
		"4834"
	}

	includedirs
	{
		"src",
		"src/alc",
		"src/common",
		"include",
		"include/AL"
	}

	files
	{
		"src/**.h",
		"src/**.cpp"
	}

	excludes
	{
		"src/alc/mixer/mixer_neon.cpp"
	}

	defines
	{
		"AL_LIBTYPE_STATIC"
	}
	
	filter "system:windows"
		systemversion "latest"
		defines "NIT_WINDOWS"

		defines
		{
			"WIN32",
			"_WINDOWS",
			"AL_BUILD_LIBRARY",
			"AL_ALEXT_PROTOTYPES",
			"_WIN32",
			"_WIN32_WINNT=0x0502",
			"_CRT_SECURE_NO_WARNINGS",
			"NOMINMAX",
			"CMAKE_INTDIR=\"Debug\"",
			"OpenAL_EXPORTS",
			"_SILENCE_NODISCARD_LOCK_WARNINGS"
		}

		links
		{
			"winmm"
		}
	
		filter "configurations:Debug"
			defines { "NIT_DEBUG" }
			symbols "On"
	
		filter "configurations:Release"
			defines { "NIT_RELEASE" }
			optimize "On"