# Rain

## Goal

This is my attempt at making a small game engine, mostly for learning purposes.

## Cloning

To clone the repo, you need to use the recursive features of git clone, some external libraries being set as submodules :

`git clone --recurse-submodules -j8 https://github.com/martinnnnnn/rain.git`

## Dependencies

You'll need to :
1. Install the FBX SDK
2. Download boost and paste its include directory in a C:/boost directory

## Building

This project is using CMake, so as pretty much every CMake projects you can simply open a shell and run cmake from the directory you want the build to be. For example on windows `cmake -G "Visual Studio 15 2017" ..` from a "build" directory.

This project has only been tested on Windows 10 with Visual Studio 2017.

## Compiling

The CMake script creates a solution containing quite a few projects on windows, the project you want to launch is "rain_engine". You will need to indicate the path to the data directory as an absolute path in the "config.rain" json file in data/exe_root ("data_root" value).

You can choose which world launches first by changing the "starting_world" value in the "config.rain" as well.

