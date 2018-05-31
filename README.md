# Rain

## Goal

This is my attempt at making a small game engine, mostly for learning purposes.

## Cloning

To clone the repo, you need to use the recursive features of git clone, some external libraries being set as submodules :

`git clone --recurse-submodules -j8 git://github.com/foo/bar.git`

## Building

This project is using CMake, so as pretty much every CMake projects you can simply open a shell and run cmake from the directory you want the build to be. For example on windows `cmake -G "Visual Studio 15 2017" ..` from a "build" directory.

This project has only been tested on Windows 10 with Visual Studio 2017.

## Compiling

The CMake script creates a solution containing quite a few projects on windows, the project you want to launch is "rain". For the engine to find the resources, you want to give it the root directory as a absolute path :

`root="D:root/to/the/repo/rain/resources"`

One more thing : when building, Assimp looks for a file "config" that it generates itself. For some reason, it can't find it while building - although it is added as a include dir in CMake - so I'm manually copying `rain/build/external/assimp/include/assimp/config.h` to `rain/external/assimp/include/assimp/`. Not ideal and the error should probably be looked at in the future, but it works as is for now.

It should then work !

## Python binding

I've started playing around with python's ctypes module to try and make an editor in python, once again mostly for learning purposes. To launch the game using python, you need to build the "rainlib" project in visual studio, then call the python script tools/editor.py

## What's left to do

Pretty much everything, it's still a really small project.

