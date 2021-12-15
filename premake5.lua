workspace "TRAE-menu-hook"
    architecture "x86"
    configurations {"Debug", "Release"}

project "TRAE-menu-hook"
    kind "SharedLib"
	targetextension ".asi"
	
    files {
		"*.hpp",
		"*.cpp",
		"**/*.cpp",
		"**/*.hpp"
	}
	
	-- ignore all vendor files in above glob
	removefiles { "vendor/**" }
	
	-- imgui files
	files {
		"vendor/imgui/*.cpp",
		"vendor/imgui/*.h",
		"vendor/imgui/examples/imgui_impl_win32.cpp",
		"vendor/imgui/examples/imgui_impl_win32.h",
		"vendor/imgui/examples/imgui_impl_dx9.cpp",
		"vendor/imgui/examples/imgui_impl_dx9.h"
	}
	
	includedirs { "vendor/imgui" }
	
	files "vendor/MinHook.h"

    language "C++"
	links { "MinHook.x86.lib", "d3d9.lib" }
	defines { "$(GameConstant)" }
	
    filter "configurations:Debug"
        defines { "DEBUG", "_DEBUG" }
        symbols "On"

    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"
        symbols "On"
