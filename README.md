# Titan Engine - Complete Game Engine & Editor

A Source/GoldSource-inspired game engine built from the ground up for better modding, performance, and future innovation. **Now includes a complete level editor with seamless engine integration!**

## 🚀 What's New (January 2026)

✅ **Complete Editor Integration** - Full-featured level editor with real-time engine preview  
✅ **Runtime DLL Loading** - Clean C++/CLI interop without static linking issues  
✅ **Production-Ready Distribution** - One-click installer with all dependencies  
✅ **Enhanced API** - 12+ new exported functions for full engine control  
✅ **Professional Build System** - Automated compilation and packaging  

## Features

### Core Architecture
- **Entity-Component System (ECS)**: Flexible, data-oriented design for easy game object management
- **Event Bus**: Decoupled systems communication through event publishing/subscription
- **System-based Architecture**: Modular, pluggable systems (rendering, physics, audio, scripting)
- **Runtime DLL Loading**: Clean .NET-C++ interop for editor integration

### Editor Integration
- **Titan Level Editor**: Complete BSP-based level editor with integrated engine preview
- **Real-time Preview**: See your game world updates live in the editor
- **Entity Management**: Create, position, and configure game objects
- **Material System**: Assign materials and textures to surfaces
- **Scripting Integration**: Lua mod support with hot reload

### Rendering
- OpenGL-based renderer with support for custom shaders
- Material system with PBR support (Metallic, Roughness, AO)
- Mesh system with vertex/index buffer management
- Debug visualization (lines, spheres)

### Physics
- Rigid body simulation with gravity
- Force application system
- Raycast support
- Velocity and acceleration tracking

### Scripting & Modding
- **Lua scripting integration** for easy mod development
- Full engine API exposed to Lua
- Dynamic mod loading/unloading
- Hot reload support

### Input System
- Full keyboard support (letters, numbers, function keys, special keys)
- Mouse input (position tracking, movement delta, button press/release)
- Scroll wheel support
- Input locking for UI interactions

### Audio System
- Audio clip management
- Volume control
- 3D positioning support
- Play/Pause/Stop/Resume controls

## Project Structure

```
TitanEngine/
├── include/          # Engine headers
│   ├── Core.hpp      # Entity, Component, Event definitions
│   ├── Engine.hpp    # Main engine class & exported functions
│   ├── Renderer.hpp  # Rendering system
│   ├── Input.hpp     # Input handling
│   ├── Physics.hpp   # Physics simulation
│   ├── Scripting.hpp # Lua integration
│   ├── Audio.hpp     # Audio system
│   ├── Window.hpp    # Window management
│   └── TitanExports.hpp # DLL export definitions
├── src/              # Engine implementation
├── example/          # Example game project
├── mods/             # User-created mods
├── tools/            # Game tools (compiler, map editor, etc.)
└── CMakeLists.txt    # Build configuration

TitanEditor/          # Complete level editor (separate project)
├── Titan.BspEditor/  # BSP editor core
├── Titan.Common/     # Shared utilities
├── Titan.EngineWrapper/ # C++/CLI interop layer
└── Titan.Shell/      # Editor UI and shell
```

## Installation & Quick Start

### Option 1: One-Click Installer (Recommended)
1. Download `TitanEngine-Installer.exe`
2. Run installer with administrator privileges
3. Launch "Titan Editor" from desktop or start menu
4. Editor starts automatically with integrated engine

### Option 2: Manual Build
```powershell
# Build Engine DLL
cd TitanEngine
mkdir build
cd build
cmake .. -G "Visual Studio 17 2022" -A x64
cmake --build . --config Release

# Build Editor (requires engine DLL)
cd ../../TitanEditor
# Use Visual Studio to build TitanEditor.sln
```

### First Run
```powershell
# Launch editor
.\Launch Titan Editor.bat

# Or run directly
.\Titan.Editor.exe
```

## Building (Advanced Users)

### Requirements
- **Visual Studio 2022** with C++ development workload
- **CMake 3.16+** 
- **.NET Framework 4.7.2+** for editor
- **Windows 10/11** (64-bit)
- **OpenGL** compatible graphics card

### Automated Build (Recommended)
```powershell
# From TitanEngine directory
.\build.bat
```

### Manual Build Process
```powershell
# 1. Build Engine DLL
cd TitanEngine
mkdir build
cd build
cmake .. -G "Visual Studio 17 2022" -A x64 -DCMAKE_BUILD_TYPE=Release
cmake --build . --config Release

# 2. Copy DLL to editor directory
copy Release\TitanEngine.dll ..\..\TitanEditor\TitanEditor\Titan.EngineWrapper\

# 3. Build Editor (open in Visual Studio)
# Open TitanEditor\TitanEditor\TitanEditor.sln
# Build Solution in Release mode

# 4. Package distribution
# Copy all built DLLs and executables to dist/ directory
# Run installer generation
```

### Linux/Mac (Experimental)
```bash
# Engine only (no editor support yet)
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make
```

## Editor Integration

### Runtime DLL Loading
The Titan Editor uses **runtime DLL loading** for seamless C++/CLI interop:

```cpp
// Engine exports (TitanEngine.dll)
extern "C" {
    TITAN_API void* CreateEngine();
    TITAN_API bool InitializeEngine(void* engine, const char* appName, int width, int height, int targetFPS, bool vsync, bool headless);
    TITAN_API void UpdateEngine(void* engine, float deltaTime);
    TITAN_API void RenderFrame(void* engine);
    // ... 12+ exported functions
}
```

```csharp
// Editor wrapper (C++/CLI)
public ref class EngineWrapper {
    void* m_engineHandle;
    // Function pointers for DLL calls
    CreateEngineFunc m_createEngine;
    UpdateEngineFunc m_updateEngine;
    // ... etc
}
```

### Editor Features
- **BSP Level Editor**: Create and edit 3D levels
- **Real-time Preview**: See engine rendering live
- **Entity Management**: Place and configure game objects
- **Material Assignment**: Apply textures and materials
- **Script Integration**: Lua mod support with hot reload

## Getting Started

### Basic Game Setup
```cpp
#include "Engine.hpp"

int main() {
    Titan::Engine engine;
    
    Titan::EngineConfig config;
    config.appName = "My Game";
    config.windowWidth = 1280;
    config.windowHeight = 720;
    
    engine.Initialize(config);
    engine.Run();
    
    return 0;
}
```

### Creating Game Objects (Entities)
```cpp
auto& entityManager = engine.GetEntityManager();

// Create entity
Titan::EntityID player = entityManager.CreateEntity("Player");
auto playerEntity = entityManager.GetEntity(player);

// Add components
auto transform = std::make_shared<Titan::Transform>(glm::vec3(0, 0, 0));
playerEntity->AddComponent<Titan::Transform>(transform);

auto rigidBody = std::make_shared<Titan::RigidBody>();
playerEntity->AddComponent<Titan::RigidBody>(rigidBody);

auto renderable = std::make_shared<Titan::Renderable>("player.mesh", "player.mat");
playerEntity->AddComponent<Titan::Renderable>(renderable);
```

### Working with Physics
```cpp
auto& physics = engine.GetPhysicsSystem();

// Set gravity
physics.SetGravity(glm::vec3(0, -9.81f, 0));

// Add physics to entity
auto body = std::make_shared<Titan::RigidBody>();
body->mass = 1.0f;
body->useGravity = true;
physics.AddRigidBody(playerID, body);

// Apply forces
body->ApplyForce(glm::vec3(0, 100, 0));
```

## Modding with Lua

### Loading a Mod
```cpp
auto& scripting = engine.GetScriptingSystem();
scripting.LoadMod("mods/my_mod.lua");
```

### Creating Lua Mods

#### Basic Mod Structure (my_mod.lua)
```lua
-- Initialize mod
function OnModInit()
    Print("My mod loaded!")
end

-- Update called every frame
function OnUpdate(deltaTime)
    Print("Delta: " .. deltaTime)
end

-- Cleanup when mod is unloaded
function OnModCleanup()
    Print("My mod unloaded!")
end
```

#### Accessing Engine APIs from Lua
```lua
-- Get engine time
local currentTime = GetTime()
local deltaTime = GetDeltaTime()

-- Create entities (when EntityAPI is fully exposed)
-- local player = CreateEntity("Player")

-- Input system (when InputAPI is fully exposed)
-- if IsKeyPressed(KeyCode.W) then
--     Print("W pressed!")
-- end

-- Physics (when PhysicsAPI is fully exposed)
-- local gravity = GetGravity()
```

## System Architecture

### Event System
```cpp
// Define custom event
struct CustomEvent : public Titan::Event {
    std::string data;
    CustomEvent(const std::string& d) : Event(2001), data(d) {}
};

// Subscribe to event
auto& eventBus = engine.GetEventBus();
eventBus.Subscribe(2001, [](const Titan::Event& event) {
    const auto& customEvent = static_cast<const CustomEvent&>(event);
    std::cout << customEvent.data << std::endl;
});

// Publish event
eventBus.Publish(CustomEvent("Hello World"));
```

### Custom Components
```cpp
class HealthComponent : public Titan::Component {
public:
    float health{100.0f};
    float maxHealth{100.0f};
    
    static constexpr Titan::ComponentID StaticID() { return 10; }
    Titan::ComponentID GetComponentID() const override { return StaticID(); }
    
    void TakeDamage(float amount) {
        health -= amount;
    }
};

// Use component
auto health = playerEntity->GetComponent<HealthComponent>();
if (health) {
    health->TakeDamage(10.0f);
}
```

### Custom Systems
```cpp
class AISystem : public Titan::ISystem {
public:
    void Initialize() override {
        std::cout << "AI System initialized" << std::endl;
    }
    
    void Update(float deltaTime) override {
        // Update all AI entities
    }
    
    void Shutdown() override {
        std::cout << "AI System shutdown" << std::endl;
    }
};

// Register system with engine
// Systems are registered in Engine::InitializeSystems()
```

## Advanced Features

### Rendering Customization
```cpp
// Set custom clear color
engine.GetRenderer().SetClearColor(glm::vec4(0.2f, 0.3f, 0.4f, 1.0f));

// Set camera matrices
auto view = glm::lookAt(
    glm::vec3(0, 2, 5),      // Camera position
    glm::vec3(0, 1, 0),      // Look at point
    glm::vec3(0, 1, 0)       // Up vector
);
engine.GetRenderer().SetViewMatrix(view);

// Debug visualization
engine.GetRenderer().DrawDebugLine(
    glm::vec3(0, 0, 0),
    glm::vec3(1, 0, 0),
    glm::vec4(1, 0, 0, 1)  // Red
);
```

### Input Handling
```cpp
auto& input = dynamic_cast<Titan::SimpleInputSystem&>(engine.GetInputSystem());

if (input.IsKeyPressed(Titan::KeyCode::W)) {
    // Move forward
}

if (input.IsMouseButtonPressed(Titan::MouseButton::Left)) {
    // Fire weapon
}

float mouseX, mouseY;
input.GetMousePosition(mouseX, mouseY);

float deltaX, deltaY;
input.GetMouseDelta(deltaX, deltaY);
```

## Future Roadmap

### ✅ Completed (January 2026)
- [x] **Complete Editor Integration** - Full BSP level editor with engine preview
- [x] **Runtime DLL Loading** - Clean C++/CLI interop architecture
- [x] **Professional Distribution** - One-click installer with all dependencies
- [x] **Enhanced API** - 12+ exported functions for full engine control
- [x] **Production Build System** - Automated compilation and packaging

### Phase 2 (Future)
- [ ] Improved physics engine integration (Bullet Physics)
- [ ] Advanced rendering (PBR materials, deferred rendering)
- [ ] Particle system
- [ ] Skeletal animation support
- [ ] Networking/Multiplayer
- [ ] More script language support (C#, Python)
- [ ] Asset pipeline and tools
- [ ] Performance profiling tools
- [ ] Advanced debugging features

## Contributing

This project is designed to be extended and improved. Areas for contribution:

1. **Systems**: Add new engine systems (AI, networking, particles)
2. **Rendering**: Improve renderer with new features
3. **Scripting**: Expand Lua API bindings
4. **Tools**: Create editor tools and asset converters
5. **Performance**: Optimize existing systems
6. **Documentation**: Expand guides and examples

## License

Titan Engine is designed for educational and commercial use. Modify and distribute as needed.

## Support

For questions and issues, refer to the example game and documentation provided.
