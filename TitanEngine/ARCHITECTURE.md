# Titan Engine - Complete Architecture Overview

## Project Summary

**Titan Engine** is a modern, modular game engine inspired by Source/GoldSource but designed from scratch for better modding support, performance, and future innovation.

### Key Features
- ✅ Entity-Component-System (ECS) architecture
- ✅ Event-driven system communication
- ✅ Lua scripting with hot-reload
- ✅ Modular physics system
- ✅ Multi-system rendering pipeline
- ✅ Input handling system
- ✅ Audio management
- ✅ Extensible component framework

## Directory Structure

```
TitanEngine/
│
├── include/                          # Public API headers
│   ├── Core.hpp                     # Entity, Component, Event, Transform, RigidBody, etc.
│   ├── Engine.hpp                   # Main engine class
│   ├── Renderer.hpp                 # Rendering system interfaces
│   ├── Input.hpp                    # Input system and events
│   ├── Physics.hpp                  # Physics system interface
│   ├── Scripting.hpp                # Lua scripting system
│   ├── Audio.hpp                    # Audio system
│   └── Window.hpp                   # Window management (Windows implementation)
│
├── src/                             # Implementation files
│   ├── Core.cpp                    # Component, Entity, EventBus implementations
│   ├── Engine.cpp                  # Main engine loop and system management
│   ├── Renderer.cpp                # OpenGL renderer implementation
│   ├── Input.cpp                   # Input system implementation
│   ├── Physics.cpp                 # Physics simulation
│   ├── Scripting.cpp               # Lua integration
│   ├── Audio.cpp                   # Audio system implementation
│   └── Window.cpp                  # Windows window creation and management
│
├── example/                         # Example game project
│   ├── main.cpp                    # Entry point
│   ├── ExampleGame.hpp             # Example game class
│   └── ExampleGame.cpp             # Game implementation with demo scene
│
├── mods/                           # Lua mod examples
│   └── example_mod.lua             # Example Lua mod showcasing scripting
│
├── tools/                          # Development tools (placeholder)
│   └── (Future: Map editor, asset compiler, etc.)
│
├── libs/                           # Third-party libraries
│   └── (Will contain dependencies)
│
├── CMakeLists.txt                  # Build configuration
├── README.md                        # Main documentation
├── DEVELOPMENT.md                   # Advanced development guide
├── QUICKSTART.md                    # Quick start guide
└── ARCHITECTURE.md                  # This file
```

## System Architecture

### 1. Core Systems

#### Entity-Component-System (ECS)
```
Entity ─┬─ Transform (position, rotation, scale)
        ├─ RigidBody (physics simulation)
        ├─ Renderable (mesh, material)
        ├─ AudioSource (sound playback)
        └─ [Custom Components]
```

Built-in Components:
- **Transform**: 3D spatial information
- **RigidBody**: Physics simulation
- **Renderable**: Visual representation
- **AudioSource**: Sound playback

#### Entity Manager
- Manages entity creation/destruction
- Component attachment/detachment
- Entity lookup and iteration

#### Event Bus
- Publish/Subscribe pattern
- Decouples systems
- Type-safe event handling

### 2. Game Systems

#### Renderer (OpenGL-based)
```cpp
class GLRenderer : public Renderer
- BeginFrame()      // Clear buffers
- SubmitMesh()      // Queue geometry
- EndFrame()        // Finalize rendering
- Present()         // Display frame
- Debug drawing (lines, spheres)
```

Features:
- Material system with PBR
- Mesh management
- Texture loading
- Debug visualization

#### Physics System
```cpp
class SimplePhysicsSystem : public PhysicsSystem
- Gravity simulation
- Rigid body dynamics
- Force application
- Raycast support
```

Features:
- Velocity and acceleration
- Mass-based forces
- Kinematic bodies
- Gravity control

#### Input System
```cpp
class SimpleInputSystem : public InputSystem
- Keyboard tracking
- Mouse position/movement
- Button press/release
- Scroll wheel support
```

Key Codes:
- Letters (A-Z)
- Numbers (0-9)
- Function keys (F1-F12)
- Arrow keys, Escape, Space, etc.

#### Scripting System
```cpp
class LuaScriptingSystem : public ScriptingSystem
- Lua VM management
- Script execution
- Function registration
- Mod loading/unloading
```

Features:
- Lua 5.x integration
- Engine API exposure
- Hot reload support
- Mod lifecycle (Init → Update → Cleanup)

#### Audio System
```cpp
class SimpleAudioSystem : public AudioSystem
- Audio clip loading
- Playback control (Play/Pause/Stop)
- Volume management
- 3D positioning
```

#### Window System
```cpp
class Win32Window : public Window
- OpenGL context creation
- Input forwarding
- Buffer swapping
- Platform-specific handling
```

## Data Flow

### Frame Cycle
```
┌─ Engine::Run()
│
├─ CalculateDeltaTime()
│  └─ Update frame timing
│
├─ UpdateSystems(deltaTime)
│  ├─ InputSystem::Update()
│  │  └─ Poll keyboard/mouse
│  ├─ PhysicsSystem::Update()
│  │  └─ Simulate rigid bodies
│  ├─ ScriptingSystem::Update()
│  │  └─ Call Lua OnUpdate()
│  ├─ AudioSystem::Update()
│  │  └─ Update audio playback
│  └─ Renderer::Update()
│     └─ Prepare frame
│
├─ RenderFrame()
│  ├─ Renderer::BeginFrame()
│  ├─ Submit entities
│  ├─ Renderer::EndFrame()
│  └─ Renderer::Present()
│
└─ [Repeat]
```

### Component Update Flow
```
Entity (Container)
  │
  ├─ Component A (Data)
  ├─ Component B (Data)
  └─ Component C (Data)
       ↓
   Systems process components
       ↓
   Event Bus notifies subscribers
```

## Extensibility Points

### 1. Custom Components
```cpp
class HealthComponent : public Titan::Component {
    static constexpr ComponentID StaticID() { return 100; }
};
```

### 2. Custom Systems
```cpp
class AISystem : public Titan::ISystem {
    void Update(float deltaTime) override;
};
```

### 3. Custom Events
```cpp
struct GameEventName : public Titan::Event {
    // Custom data
};
```

### 4. Lua Mods
```lua
function OnModInit() end
function OnUpdate(deltaTime) end
function OnModCleanup() end
```

## Design Patterns Used

### 1. Entity-Component-System
- Separates data (components) from behavior (systems)
- Enables composition over inheritance
- Improves cache locality and performance

### 2. Event Bus / Observer Pattern
- Systems communicate through events
- Reduces coupling between systems
- Enables plugin architecture

### 3. Factory Pattern
- EntityManager creates entities
- ScriptingSystem loads mods
- Consistent object creation

### 4. Singleton Pattern
- Global Engine instance via GetEngine()
- Single EntityManager, EventBus, etc.
- Consistent system access

### 5. Component Pattern
- Entities are containers
- Components hold state
- Reusable building blocks

### 6. Template Method Pattern
- ISystem::Update() defines lifecycle
- Subclasses implement specific behavior

## Performance Characteristics

### Time Complexity
- Entity creation: O(1)
- Component access: O(1)
- System iteration: O(n) where n = entities with component type
- Event publishing: O(m) where m = subscribers

### Space Complexity
- Entity storage: O(n)
- Component storage: O(m) where m = total components
- Event subscribers: O(e) where e = event types

### Optimization Opportunities
1. Spatial partitioning for physics queries
2. Component pooling for frequent allocations
3. System ordering for cache efficiency
4. Lazy loading for large assets
5. Parallel system updates

## Building and Running

### Dependencies
- CMake 3.16+
- C++17 compiler
- OpenGL libraries
- Lua development files

### Build Steps
```bash
mkdir build
cd build
cmake ..
cmake --build . --config Release
```

### Running
```bash
./Release/TitanGame.exe  # Example game
```

## Future Roadmap

### Phase 1 (Current)
- ✅ Core ECS framework
- ✅ Basic rendering
- ✅ Physics simulation
- ✅ Lua scripting
- ✅ Input handling

### Phase 2 (Planned)
- [ ] Advanced rendering (deferred, PBR)
- [ ] Bullet Physics integration
- [ ] Skeletal animation
- [ ] Particle system
- [ ] Advanced debugging tools

### Phase 3 (Planned)
- [ ] Networking support
- [ ] Audio engine (3D spatial)
- [ ] Editor integration
- [ ] C# scripting support
- [ ] Asset pipeline tools

### Phase 4 (Planned)
- [ ] Multiplayer framework
- [ ] Advanced AI systems
- [ ] Procedural generation
- [ ] VR support
- [ ] Performance profiler

## Testing Strategy

### Unit Tests (To be added)
- Component behavior
- System updates
- Entity management

### Integration Tests (To be added)
- Multi-system interactions
- Event bus communication
- Lua script execution

### Performance Tests (To be added)
- Entity iteration speed
- Memory usage patterns
- Frame timing consistency

## Documentation Structure

1. **README.md** - Overview and quick links
2. **QUICKSTART.md** - 5-minute setup guide
3. **DEVELOPMENT.md** - Advanced concepts and patterns
4. **ARCHITECTURE.md** - This document
5. **Code Comments** - Inline documentation

## Contributing

Areas for contribution:
1. **Rendering**: Improve renderer features
2. **Physics**: Integrate advanced physics engines
3. **Tools**: Create editor and asset tools
4. **Performance**: Optimize existing systems
5. **Documentation**: Expand guides and examples
6. **Scripting**: Add more engine API bindings

## License

Titan Engine - Available for educational and commercial use.

---

For detailed information, see README.md, QUICKSTART.md, or DEVELOPMENT.md.
