# TRAE-menu-hook

Reverse engineering project for Tomb Raider LAU games, this repo contains the code for TRAE-menu-hook menu for both Tomb Raider Anniversary and Underworld.

## Videos

(screenshots at bottom of readme)

| [![](https://i.imgur.com/S8JnQjP.png)](https://www.youtube.com/watch?v=Bzf8k7LO5uI) | [![](https://i.imgur.com/qgDQuio.png)](https://www.youtube.com/watch?v=k1FIa8Pel3E&t) | [![](https://i.imgur.com/cJe3pYa.png)](https://www.youtube.com/watch?v=RwU80Pj0PR8) | [![](https://i.imgur.com/fiSP0Dp.png)](https://www.youtube.com/watch?v=Ju8Hr8Rfz4k) | [![](https://i.imgur.com/ZzRMr2P.png)](https://www.youtube.com/watch?v=bXgBKVIS9cY) |
|----|----|----|----|----|
| Menu in Underworld | Playtrough with level debug prints | Instance viewer showcase | Brought back Binoculars feature | Menu in Anniversary |

## How to

Use [Ultimate ASI Loader](https://github.com/ThirteenAG/Ultimate-ASI-Loader/releases) by replacing `binkw32.dll` and drop the compiled .asi in your game folder.

# Build

Make sure you have [premake5](https://premake.github.io/) installed and in your PATH then run
```
premake5 vs2019
```
Open the solution file and build the project. Make sure you have [MinHook.x86.lib](https://github.com/TsudaKageyu/minhook/releases) and d3d9

Make sure to set TRAE or TR8 preprocessor definition for the target game.

## Todo

- Make use of patterns instead of hardcoded addresses
- Support Steam version for Anniversary

![image](https://user-images.githubusercontent.com/15322107/105777010-ed28a200-5f69-11eb-9a2b-dfc3f8ec3293.png)
![image](https://user-images.githubusercontent.com/15322107/110152916-a2a90980-7de2-11eb-8da4-ba28e1f4a258.png)