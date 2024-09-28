# Compiling

Chimp uses CMake to fetch and link dependencies. You will not need to download anything.

## How is the engine linked to the game?

The engine is an executable and the game is a statically linked library.
Chimp (the engine) and ChimpGame (the game) are separate projects.

## PC Requirements

- 64-bit Windows
- GPU supports at least OpenGL 4.6

## How do I build it then?

Simply use visual studio to run the "Chimp.exe" target.
You may need to generate the build configurations prior to this, which can be done by reloading both CMakeLists.txt files.