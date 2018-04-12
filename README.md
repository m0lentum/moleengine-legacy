## Synopsis
MoleEngine is a 2D game engine library written in C++ on top of SFML, featuring game state and asset management,
physics simulation and various animation tools. It is early in development and not ready for any serious use yet.

Currently implemented features:
* Game state management
* Texture loading
* Game objects and object management
* Game object architecture based on the Entity-Component-System model (https://en.wikipedia.org/wiki/Entity%E2%80%93component%E2%80%93system)
* Separate update loops with fixed and uncapped framerate to separate game logic and rendering
* Collision detection for circles, rectangles and convex polygons
* Sprite-sheet animation
* Ability to easily create development and debugging tools using ImGui

Planned features:
* Optimization of collision detection via spatial partitioning
* Physics simulation for rigid bodies
* Level data format for loading scenes from file
* Base classes for a user interface
* Keyframe-based animation of various object properties
* Multithreaded optimization where applicable

## 3rd Party Libraries
SFML (https://www.sfml-dev.org/)
Dear ImGui (https://github.com/ocornut/imgui)
+ ImGui SFML Binding (https://github.com/eliasdaler/imgui-sfml)

## Installation
Note: You probably shouldn't yet. Here are the instructions nonetheless.

**Windows (VS2017 method)**

1. Install SFML.
2. Clone this repository.
3. Open the repo's root folder in Visual Studio 2017. It should run CMake automatically.
4. Go to CMake -> Change CMake Settings -> CMakeLists.txt and add the following variables to your build configurations:
```
"variables": [
              {
                "name": "SFML_ROOT",
                "value": // your SFML install folder if it is not in your system PATH
              },
              {
                "name": "CMAKE_INSTALL_PREFIX",
                "value": // directory to install the project in
              }
            ]
```
5. Run CMake -> install -> CMakeLists.txt.
6. The library should now be in your install directory, ready to be linked to your projects.

**Note:** this library is statically linked, but the SFML .dlls are required by projects using it. I will look into
different linking options in the future.

**Linux**

1. Install CMake (https://cmake.org/).
2. Install SFML.
3. Clone this repository.
4. Create a build directory anywhere outside of the repository.
5. Navigate to the directory and run\
```cmake -DCMAKE_INSTALL_PREFIX="/path/to/install/location" -DCMAKE_BUILD_TYPE=("Debug" or "Release") /path/to/engine/root```\
Additionally, if SFML is not in your system path, set `-DSFML_ROOT="/path/to/sfml"`
6. Run `make install`
7. The library should now be in your install directory, ready to be linked to your projects.

## Usage
Basic tutorials will be added once the project is in a usable state.\
For now you can refer to [my very messy test game](https://github.com/MoleTrooper/MoleEngineTestGame) for hints.

## Contact
If you have comments, suggestions or anything else, you can reach me via Discord (MoleTrooper#6910), email (mikael.myyrae@gmail.com)
or Twitter (https://www.twitter.com/moletrooper).
