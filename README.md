## Synopsis
MoleEngine is a 2D game engine library written in C++ on top of SFML, featuring game state and asset management,
physics simulation and various animation tools. It is early in development and not ready for any serious use yet.

Currently implemented features:
* Game state management
* Texture loading
* Game objects and object grouping
* Fixed and uncapped framerate update loops to separate game logic and rendering
* Some (work-in-progress) collision detection capabilities
* Sprite-sheet animation

Planned features:
* Optimization of collision detection via spatial partitioning
* Physics simulation for rigid bodies
* Level data format
* Base classes for a user interface
* Keyframe-based animation of various object properties

## Dependencies
SFML (https://www.sfml-dev.org/)

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

**Note:** this library is statically linked, but the SFML .dlls are required by projects using it.

**Linux**

(coming soon)

## Usage
Basic tutorials coming soonish, probably.\
For now you can refer to [my very messy test game](https://github.com/MoleTrooper/MoleEngineTestGame) for hints.

## Contact
If you have comments, suggestions or anything else, you can reach me via Discord (MoleTrooper#6910), email (mikael.myyrae@gmail.com)
or Twitter (https://www.twitter.com/moletrooper).
