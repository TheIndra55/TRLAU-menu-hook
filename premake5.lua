workspace "TRAE-menu-hook"
    architecture "x86"
    configurations { "Debug", "Release" }
	platforms { "TR7", "TR7DEBUG", "TRAE", "TR8" }

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
		"vendor/imgui/backends/imgui_impl_win32.cpp",
		"vendor/imgui/backends/imgui_impl_win32.h",
		"vendor/imgui/backends/imgui_impl_dx9.cpp",
		"vendor/imgui/backends/imgui_impl_dx9.h"
	}
	
	includedirs { "vendor/imgui", "vendor/json/include" }
	
	files "vendor/MinHook.h"

    language "C++"
	links { "d3d9.lib" }
	
	-- Xinput disabled since it causes issues with Windows 7
	defines { "IMGUI_IMPL_WIN32_DISABLE_GAMEPAD", "GENERAL_FIXES" }
	
    filter "configurations:Debug"
        defines { "DEBUG", "_DEBUG" }
        links { "libMinHook-mdd-x86.lib" }
        symbols "On"

    filter "configurations:Release"
        defines { "NDEBUG" }
        links { "libMinHook-md-x86.lib" }
        optimize "On"
        symbols "On"

	filter "platforms:TR7"
		defines { "TR7", "RETAIL_VERSION" }
		targetname "TR7-Menu-Hook"
		
	filter "platforms:TR7DEBUG"
		defines { "TR7" }
		targetname "TR7DEBUG-Menu-Hook"
		
	filter "platforms:TRAE"
		defines { "TRAE" }
		targetname "TRAE-Menu-Hook"
		
	filter "platforms:TR8"
		defines { "TR8" }
		targetname "TR8-Menu-Hook"