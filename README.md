# Tetris

This ripository contains my own Tetris game using SDL2 library for graphics.
Tutorial from gametuto.com by Javier López López was taken as base.

## How to run Tetris using Visual Studio 2022

First of all we need to clone repository on the local machine.

```
git clone https://github.com/Grasmus/Tetris.git
```

Open the solution in Visual Studio 2022 and go to Project->Properties.
Then open Configuration Properties->C/C++->General.
Click on Additional Include Directories and edit them.
In opened window you need to add paths to SDL2, SDL2_ttf and SDL2_image include directories.

In the Linker->General edit Additional Library Directories.
Here you need to add paths to lib folders for SDL2, SDL2_ttf and SDL2_image libraries.

Also in the Linker->Input edit Additional Depedencies.
Here you need to add opengl32.lib dependency and lib dependencies for SDL2, SDL2_ttf and SDL2_image libraries.

For example, path to lib dependency for x64 version of SDL2 library looks like this: SDL2\lib\x64\SDL2.lib and SDL2\lib\x64\SDL2main.lib.

Same paths need to be added to Additional Depedencies for SDL2_ttf and SDL2_image libraries.


After this go to Linker->System and change SubSysytem to Windows.

If all done correctly you should be able to run game without linking or dependency errors, but there will be three messages after compilation about missing dll files.

All you need to resolve this issue is to copy all the .dll files to the root folder of your repository from lib/x64 or x86 folder.
