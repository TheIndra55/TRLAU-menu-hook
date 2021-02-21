# TRAE-menu-hook

Reverse engineering Tomb Raider: Anniversary and using that research to create a functional debug/mod menu.

[Video](https://www.youtube.com/watch?v=RwU80Pj0PR8)
[Video](https://www.youtube.com/watch?v=bXgBKVIS9cY)

## How to

Use [Ultimate ASI Loader](https://github.com/ThirteenAG/Ultimate-ASI-Loader/releases) by replacing `binkw32.dll` and drop the compiled .asi in your game folder.

# Build

Make sure you have [premake5](https://premake.github.io/) installed and in your PATH then run
```
premake5 vs2019
```
Open the solution file and build the project. Make sure you have [MinHook.x86.lib](https://github.com/TsudaKageyu/minhook/releases) and d3d9

## Todo

- Uses memory addresses (no patterns)
- Hooks don't work on Steam version

![image](https://user-images.githubusercontent.com/15322107/105777010-ed28a200-5f69-11eb-9a2b-dfc3f8ec3293.png)
