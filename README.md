# TRAE-menu-hook

Reverse engineering project for Tomb Raider LAU games, this repo contains the code for TRAE-menu-hook menu for Tomb Raider Anniversary, Legend and Underworld.

## Videos

(screenshots at bottom of readme)

| [![](https://i.imgur.com/S8JnQjP.png)](https://www.youtube.com/watch?v=Bzf8k7LO5uI) | [![](https://i.imgur.com/qgDQuio.png)](https://www.youtube.com/watch?v=k1FIa8Pel3E&t) | [![](https://i.imgur.com/cJe3pYa.png)](https://www.youtube.com/watch?v=RwU80Pj0PR8) | [![](https://i.imgur.com/fiSP0Dp.png)](https://www.youtube.com/watch?v=Ju8Hr8Rfz4k) | [![](https://i.imgur.com/ZzRMr2P.png)](https://www.youtube.com/watch?v=bXgBKVIS9cY) |
|----|----|----|----|----|
| Menu in Underworld | Playtrough with level debug prints | Instance viewer showcase | Brought back Binoculars feature | Menu in Anniversary |

## Features

### Tomb Raider: Anniversary

**Supported version**: 4.58MB exe, hooks probably don't work with Steam version

* Skew/flight cheat
* Restore full health
* Switch player character
* Binoculars from Legend brought back
* Disable level script
* Debug log/draw from levels restored
* Enable debug keypad to trigger debug input actions (e.g. killing bosses, skipping depending on level)
* Wireframe rendering
* Entity ESP to show entities (including intro id, "family", health and pointer address)
* Switch chapter
* Switch unit
* Loading units from a folder outside of bigfile.000
* List units
* List instances in a window with properties
* Turn lara into gold
* Give items and weapons
* Spawn instances (entities)
* Delete instances
* Load replacement or new objects outside bigfile.000
* Load objectlist.txt outside bigfile.000
* Load .raw files outside bigfile.000
* Free camera
* See info about instance models
* Play animations

### Tomb Raider: Underworld

**Supported version**: Steam \
Progress on Underworld was started later and this is also a newer version of the engine so requires more research, therefore Underworld version is less feature rich.

* Skew/flight cheat
* Switch player character
* Switch unit
* List units
* Debug log from level scripts restored
* Loading units from a folder outside of bigfile.000

### Tomb Raider: Legend

**Supported version**: nextgen demo debug exe

* Skew/flight cheat
* Switch player character
* Switch unit
* List units
* List instances
* Spawn instances
* Delete instances
* Load replacement or new objects outside bigfile.000
* Load objectlist.txt outside bigfile.000
* Load .raw files outside bigfile.000
* See info about instance models
* Play animations

## How to

Use [Ultimate ASI Loader](https://github.com/ThirteenAG/Ultimate-ASI-Loader/releases) by replacing `binkw32.dll` and drop the compiled .asi in your game folder.

## Build

Make sure you have [premake5](https://premake.github.io/) installed and in your PATH then run
```
premake5 vs2019
```
Open the solution file and build the project. Make sure you have [MinHook.x86.lib](https://github.com/TsudaKageyu/minhook/releases) and d3d9

Make sure to set TRAE, TR7 or TR8 preprocessor definition for the target game.

## Todo

Other todo items can be found in the 'Projects' tab on GitHub

- Make use of patterns to support more executables like Steam

## FAQ

### How to load new objects

First browse to the root of the disk where your game is installed, for example D:\

Add a folder D:\units here your will put the replacement or new .drm files \
If you are also adding new files copy the game's existing objectlist.txt to D:\TRAE\pc-w\objectlist.txt, higher the first number by one and add another line with your id and name.

**Note** objectlist.txt must always end with a line ending (empty line) else the game will crash on start.

### Controls

<kbd>F7</kbd> Hide the menu \
<kbd>F8</kbd> Toggle menu focus, this allows you to interact with the menu \
<kbd>F9</kbd> Switch player character 

<kbd>F2</kbd> Toggle skew/flight cheat \
<kbd>F3</kbd> Freeze the game \
<kbd>F4</kbd> Toggle free camera 

<kbd>Q</kbd> Flight up \
<kbd>Z</kbd> Flight down

## Screenshots

![image](https://user-images.githubusercontent.com/15322107/111395421-1e496700-86bd-11eb-997b-b73f2a3ec244.png)
![image](https://user-images.githubusercontent.com/15322107/110152916-a2a90980-7de2-11eb-8da4-ba28e1f4a258.png)
![image](https://user-images.githubusercontent.com/15322107/112666334-5ccce780-8e5c-11eb-8592-4ccc47627dba.png)
