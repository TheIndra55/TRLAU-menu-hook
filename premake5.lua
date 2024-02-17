workspace "TRAE-menu-hook"
    architecture "x86"
    configurations { "Debug", "Release" }
    platforms { "TR7", "TRAE", "TR8" }

project "TRAE-menu-hook"
    kind "SharedLib"
    targetextension ".asi"

    language "C++"
    cppdialect "C++17"

    links { "d3d9.lib" }

    -- Source files'
    files "src/**"
    includedirs { "src" }

    -- Vendor files
    files {
        "vendor/minhook/src/**",
        "vendor/patterns/*.cpp",
        "vendor/imgui/*.cpp",
        "vendor/imgui/backends/imgui_impl_win32.cpp",
        "vendor/imgui/backends/imgui_impl_dx9.cpp"
    }

    includedirs {
        "vendor/minhook/include",
        "vendor/patterns",
        "vendor/imgui",
        "vendor/imgui/backends",
        "vendor/json/include"
    }

    defines { "IMGUI_IMPL_WIN32_DISABLE_GAMEPAD" }

    -- Build configurations
    symbols "On"

    filter "configurations:Debug"
        defines { "DEBUG", "_DEBUG" }

    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"

    -- Game configurations
    filter "platforms:TR7"
        defines { "TR7" }
        targetname "TR7-Menu-Hook"

    filter "platforms:TRAE"
        defines { "TRAE" }
        targetname "TRAE-Menu-Hook"

    filter "platforms:TR8"
        defines { "TR8" }
        targetname "TR8-Menu-Hook"
