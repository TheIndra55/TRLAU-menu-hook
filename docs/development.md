# Development

## Prerequisites

- [Premake](https://premake.github.io/download)

## Building

Clone the repository with all submodules.

```bash
git clone https://github.com/TheIndra55/TRAE-menu-hook
cd TRAE-menu-hook
git submodule update --init
```

Next generate the project files with Premake, for example for Visual Studio:

```bash
premake5 vs2022
```

Now open the solution (.sln) and build for the preferred game.

## Modules

The codebase uses modules for adding new functionality or menus. Modules are implemented by inheriting the `Module` class.

### Adding a module

To add a new module create a class inheriting `Module`. You can 

```cpp
#include "Module.h"

class MyModule : public Module
{
}
```

Then register the module in `Hook.cpp` in `RegisterModules`.

```cpp
void Hook::RegisterModules()
{
    ...

    RegisterModule<MyModule>();
}
```

### Getting a module

In case you want to interact with a module somewhere else in the code, you can get the module instance.

```cpp
auto log = Hook::GetInstance().GetModule<Log>();

log->WriteLine("Hello, World!");
```

### Abstract methods

Modules can implement some abstract methods to be called during a stage.

```cpp
class MyModule : public Module
{
public:
    void OnFrame()
    {
        auto font = Font::GetMainFont();

        font->SetCursor(0.f, 0.f);
        font->Print("Hello, World!");
    }
}
```

#### OnMenu

Called while the main menu is being drawn, this can be used for adding new menu items.

```cpp
void MyModule::OnMenu()
{
    if (ImGui::BeginMenu("Your menu"))
    {
        ImGui::MenuItem("Your menu item");

        ImGui::EndMenu();
    }
}
```

#### OnDraw

Called during an ImGui frame, use this to draw your menus.

#### OnFrame

Called just before a frame ends, use this to use font or draw functions.

#### OnLoop

Called every frame before the game loop.

#### OnInput

Called for every message from the window procedure.

```cpp
void MyModule::OnInput(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (msg == WM_KEYUP && wParam == VK_F1)
    {
        // F1 pressed!
    }
}
```

#### OnPostInitialize

Called on post initialization after the device has been obtained
