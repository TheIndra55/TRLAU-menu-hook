# TRAE-menu-hook

Reverse engineering project for Tomb Raider LAU games, this repo contains the code for TRAE-menu-hook menu for Tomb Raider Anniversary, Legend and Underworld.

[![](https://github.com/TheIndra55/TRAE-menu-hook/actions/workflows/build.yml/badge.svg)](https://github.com/TheIndra55/TRAE-menu-hook/actions/workflows/build.yml)

## Videos

(screenshots at bottom of readme)

| [![](https://i.imgur.com/RMnCPck.png)](https://www.youtube.com/watch?v=orv2mYjBNhM) | [![](https://i.imgur.com/qgDQuio.png)](https://www.youtube.com/watch?v=k1FIa8Pel3E&t) | [![](https://i.imgur.com/cJe3pYa.png)](https://www.youtube.com/watch?v=RwU80Pj0PR8) | [![](https://i.imgur.com/MEPYZBD.png)](https://www.youtube.com/watch?v=BfUCcC6z1jo) | [![](https://i.imgur.com/8HvLmPG.png)](https://www.youtube.com/watch?v=JvH0RPbaUI0) |
|----|----|----|----|----|
| Menu in Underworld | Playtrough with level debug prints | Instance viewer showcase | Loading .drm files without bigfile | Menu in Anniversary |

## Features

### Tomb Raider: Anniversary

**Supported version**: 4.58MB exe, Steam version

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
* Slow down game (slow motion)
* See level markup
* See enemy navigation
* See collision mesh
* See portals
* Skip intros/copyright screen
* Disable death fade/respawn
* Disable cinematic bars

### Tomb Raider: Underworld

**Supported version**: Steam \
Progress on Underworld was started later and this is also a newer version of the engine so requires more research, therefore Underworld version is less feature rich.

* Skew/flight cheat
* Switch player character
* Switch unit
* List units
* Debug log from level scripts restored
* Loading units from a folder outside of bigfile.000
* List instances
* Spawn instances
* Delete instances
* Load replacement objects outside bigfile.000
* Play animations
* Slow down game (slow motion)
* Free camera
* Disable death fade/respawn

### Tomb Raider: Legend

**Supported version**: nextgen demo debug exe

* Skew/flight cheat
* Switch player character
* Switch unit
* List units
* List instances
* Spawn instances
* Delete instances
* Entity ESP to show entities (including intro id, "family", health and pointer address)
* Loading units from a folder outside of bigfile.000
* Load replacement or new objects outside bigfile.000
* Load objectlist.txt outside bigfile.000
* Load .raw files outside bigfile.000
* See info about instance models
* Play animations
* Slow down game (slow motion)
* See level markup
* See enemy navigation
* Wireframe rendering
* See collision mesh
* See portals
* Disable death fade/respawn
* Disable cinematic bars

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
- Refactor a lot of code to use structs instead of offsets

## FAQ

### How to load files outside bigfile.000

Create a folder at the root of your disk \TRAE\units on which the game is installed for example D:\TRAE\units and add the files.

|Game         | Folder | Support                                        |
|-------------|--------|------------------------------------------------|
| Anniversary | \TRAE\ | .raw, .drm (units and objects), objectlist.txt |
| Legend      | \TR7\  | .raw, .drm (units and objects), objectlist.txt |
| Underworld  | \TR8\  | .drm (units and objects)                       |

Objects have to be defined in objectlist.txt to birth them ingame (see below), units don't and can always be loaded.

### How to load custom objectlist.txt

Use the same folder as above but instead of \units create a folder \pc-w and put your objectlist.txt inside, if it finds your objectlist.txt it will tell so in the Log window.

### How to add new objects

Start by adding the .drm file of your object to the units folder (see above), next open your objectlist.txt and add +1 to the number on the first line. Then scroll to the last line and add a new line
```
959,yourobjectwithoutdotdrm
```

**Note** objectlist.txt must always end with a line ending (empty line) else the game will crash on start.

### Controls

<kbd>F7</kbd> Hide the menu \
<kbd>F8</kbd> Toggle menu focus, this allows you to interact with the menu \
<kbd>F9</kbd> Switch player character \
<kbd>F11</kbd> Instant ragdoll death

<kbd>F2</kbd> Toggle skew/flight cheat \
<kbd>F3</kbd> Freeze the game \
<kbd>Shift</kbd> + <kbd>F3</kbd> Cycle Slow motion \
<kbd>F4</kbd> Toggle free camera \
<kbd>F5</kbd> Toggle player control

<kbd>Q</kbd> \/ <kbd>A</kbd> Flight up \
<kbd>Z</kbd> \/ <kbd>W</kbd> Flight down

Flight controls depend on QWERTY/AZERTY keyboard layout

### Thanks

* SunBeam906
* Xwilarg

## Screenshots

![image](https://user-images.githubusercontent.com/15322107/111395421-1e496700-86bd-11eb-997b-b73f2a3ec244.png)
![image](https://user-images.githubusercontent.com/15322107/113633351-19992280-966d-11eb-9924-27cb87a3830f.png)
![image](https://user-images.githubusercontent.com/15322107/112666334-5ccce780-8e5c-11eb-8592-4ccc47627dba.png)
