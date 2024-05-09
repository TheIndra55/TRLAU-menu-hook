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

    -- Source files
    files "src/**"
    includedirs { "src" }

    -- Vendor files
    dofile "vendor.lua"

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

project "Tests"
    kind "ConsoleApp"

    language "C++"
    cppdialect "C++17"

    files {
        "tests/**",
        "src/**",
        "vendor/catch2/extras/catch_amalgamated.cpp"
    }

    includedirs { "src", "vendor/catch2/extras" }
    dofile "vendor.lua"

    defines { "TR7" }