# Mods

A mod loader is provided out of the box, this allows you to replace game files without modifying the game archives.

## Mods folder

The mods folder, located in the game folder contains all overridden files. Files which are normally in the PC-W folder such as lara.drm are located in the root of the mods folder, other locations such as `\trae\pc\objectlist.txt` have been relocated to the mods folder.

## Specialisation

For localized files such as locals.bin the language mask can be appended after the file name. For example for the French language mask (2) this will be `locals.bin_002`.

## Examples

Below are some example paths mapped to mods folder.

| Original path | Mods folder path |
|---------------|------------------|
| `pc-w\lara.drm` | `mods\lara.drm` |
| `pc-w\sndstrm\ui\trae_main_theme_2.mul` | `mods\sndstrm\ui\trae_main_theme_2.mul` |
| `\trae\pc-w\objectlist.txt` | `mods\trae\pc-w\objectlist.txt` |
