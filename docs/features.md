# Features

There is a wide range of features, this page explains all the features.

## Controls

The following keys can be used ingame.

| Key | Description |
|-----|-------------|
| <kbd>F8</kbd> | Shows the menu |
| <kbd>F2</kbd> | Toggles the skew/flight cheat |
| <kbd>F3</kbd> | Freezes the games |
| <kbd>F4</kbd> | Toggles the free camera mode |
| <kbd>F5</kbd> | Toggles player control |
| <kbd>F11</kbd> | Instant ragdoll death |

### Skew

The skew cheat can be used to fly the player freely through the world. To move the player up or down use the <kbd>Q</kbd> and <kbd>Z</kbd> keys (or  <kbd>A</kbd> and <kbd>W</kbd> on AZERTY).

## Free camera

The free camera can be used to freely fly the camera.

### Modes

The free camera can be used in two modes. The first mode freezes player control and allows you to fly the camera, the second mode disables the camera control and allows you to control the player again. At any moment you can press <kbd>F5</kbd> to toggle the player control.

### Controls

The following controls are used by the free camera

| Key | Description |
|-----|-------------|
| WASD | Move the camera forward, backwards, left or right |
| <kbd>Shift</kbd> | Fast camera speed |
| <kbd>Ctrl</kbd> | Slow camera speed |
| <kbd>Q</kbd>/<kbd>E</kbd> | Move the camera up/down |
| <kbd>1</kbd>/<kbd>3</kbd> | Roll the camera |

## Options

The options menu under help provides some options for toggling features or changing defaults.

## Menu

This menu is always visible and contains some basic or quick options.

### Load unit

This allows you to switch to a different unit (level).

### Birth object

This allows you to spawn an instance with the object name.

### Player

This section contains some options related to the player.

#### Fill 'er up

This fills up your health to the max health, the name of this option is based on the real debug menu.

#### Outfit

This allows you to change the player outfit, press the next button to go through the outfits.

#### No interpolation

This disables animation interpolation.

#### No death fade

This disables the fade on death, meaning the player won't respawn after dying. Disable this option again to respawn.

### Time

This section allows you to change the time multiplier for example to slow down the game or speed it up.

### Save

This section allows you to change saved data, currently this only contains the ability to set event variables.

## Instances

The instance viewer (instances) shows all the current instances in the game.

![](https://github.com/TheIndra55/TRAE-menu-hook/assets/15322107/de6f20a3-dbda-4d3f-ada4-377fbfc0b1f1)

### Transforms

This section can be used to change the position and rotation of an instance.

### Object

This section shows some basic info about the object of the instance.

### Draw groups

This section shows the current draw groups and allows you to toggle draw groups on the model.

### Animations

This sections shows the current playing animations on the instance and allows you to play animations.

### Messaging

This section is used for posting messages to the instance.

## Level

The level menu contains some options relating to levels.

### Disable script

Checking this option will disable the loading of the level script.

### Event debug

This window allows you to see the current event (script) variables for a level.

![](https://github.com/TheIndra55/TRAE-menu-hook/assets/15322107/27ffc85e-85e5-4b9e-aa49-cd3a920a7ec7)

## Draw

The draw menu can be used to enable various of debug drawing such as drawing of the collision mesh or signals.

![](https://github.com/TheIndra55/TRAE-menu-hook/assets/15322107/889b8c07-c451-444a-8a7a-ab8b8dcd511d)

### Draw options

This window will be visible when drawing instances or the collision mesh and allows you to specify some filters.

![](https://github.com/TheIndra55/TRAE-menu-hook/assets/15322107/a39685d0-8f2f-4a82-8caa-aea2d29534e6)

### Draw instances

This will draw a text with the instance name on the world position of all instances.

### Draw markup

This will draw a visualization of all markup, such as ledges.

### Draw enemy route

This will visualize the pathfinding route of all enemies.

### Draw collision

This will visualize the collision mesh of the current level.

### Draw portals

This will visualize all portals which are between levels.

### Draw signals

This will draw all signals (triggers) in the current level.

## Debug

The debug menu has some restored event debug features.

### Draw debug

This will enable all of the debug drawing from the level.

### Debug keypad

This will enable the debug keypad, which allows you to use debug key combinations.

## Render

The render menu allows toggling some render modes such as wireframe and terrain wireframe.

## Frontend

The frontend menu allows you to hide the frontend HUD.