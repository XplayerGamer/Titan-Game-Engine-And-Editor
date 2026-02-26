# Titan Game Engine & Editor

A complete game development environment featuring a powerful C++ game engine with integrated level editor, similar to GoldSource (Half-Life) Hammer Editor.

## Features

### Game Engine
- **Vulkan Rendering**: Modern graphics API with high performance
- **Bullet Physics**: Realistic physics simulation
- **Lua Scripting**: Dynamic gameplay scripting
- **Entity Component System**: Flexible game object management
- **Audio System**: Sound and music support
- **Networking**: Multiplayer capabilities
- **Performance Monitoring**: Real-time performance analysis

### Level Editor
- **BSP Editor**: Create complex 3D levels like GoldSource
- **Real-time Preview**: See changes instantly with engine integration
- **Physics Simulation**: Test physics in-editor
- **Scripting Integration**: Execute Lua scripts during editing
- **Multiple Viewports**: 3D, 2D top/down, front/side views
- **Texture Browser**: Comprehensive texture management
- **Entity Placement**: Place and configure game objects

## System Requirements

- Windows 10 or later (64-bit)
- Vulkan-compatible graphics card
- 4GB RAM minimum, 8GB recommended
- 2GB free disk space

## Installation

1. Run the installer or extract the archive
2. Launch "Launch Titan Editor.bat"
3. The editor will start with the Titan Game Engine initialized

## Getting Started

### Creating a New Map
1. Open the editor
2. File -> New Map
3. Use the brush tools to create geometry
4. Add entities from the entity browser
5. Apply textures using the texture browser
6. Test physics with the physics simulation tools

### Scripting
- Use Lua for gameplay logic
- Scripts can be executed in-editor for testing
- Full integration with engine systems

### Building Your Game
- Export maps and assets
- Use the engine library to create standalone games
- Integrate with your own C++ code

## Architecture

The system consists of:
- **TitanEngine.lib**: Core C++ game engine
- **Titan.EngineWrapper.dll**: C++/CLI bridge for .NET integration
- **Titan.Editor.exe**: Main editor application
- **Lua54.dll**: Lua scripting runtime

## Development

This is a complete game development toolkit that combines:
- Professional-grade C++ engine
- User-friendly level editor
- Modern rendering and physics
- Scripting capabilities

Perfect for indie game development, educational purposes, or as a foundation for commercial projects.

## License

Copyright 2026 - Titan Game Engine Team
All rights reserved.