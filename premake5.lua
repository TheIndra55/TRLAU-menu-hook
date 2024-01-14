workspace "TRAE-menu-hook"
    architecture "x86"
    configurations { "Debug", "Release" }

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

    -- Build configurations
    symbols "On"

    filter "configurations:Debug"
        defines { "DEBUG", "_DEBUG" }

    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"