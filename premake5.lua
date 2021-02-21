workspace "TRAE-menu-hook"
    architecture "x86"
    configurations {"Debug", "Release"}

project "TRAE-menu-hook"
    kind "SharedLib"
	targetextension ".asi"
	
    files {
		"*.hpp",
		"*.cpp",
		"*.h",
		"include/**"
	}
    language "C++"
	links { "MinHook.x86.lib", "d3d9.lib" }
	
    filter "configurations:Debug"
        defines { "DEBUG", "_DEBUG" }
        symbols "On"

    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"
        symbols "On"
		linkoptions { "/pdbaltpath:%_PDB%" }
