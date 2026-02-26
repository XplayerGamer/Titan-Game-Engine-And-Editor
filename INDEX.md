# Titan Engine - Getting Started Index

Welcome to **Titan Engine** - A modern game engine built for innovation and modding!

## 📖 Where to Start

### 1️⃣ **New to the Engine?** → Start Here
- 📄 **[PROJECT_SUMMARY.md](PROJECT_SUMMARY.md)** - Complete overview of what's included
- 🚀 **[QUICKSTART.md](QUICKSTART.md)** - Get running in 5 minutes
- 📚 **[README.md](README.md)** - Full feature documentation

### 2️⃣ **Ready to Build?** → Build the Engine
```powershell
cd TitanEngine
.\build.bat
```

### 3️⃣ **Want to Learn?** → Read Documentation
- 🏗️ **[ARCHITECTURE.md](ARCHITECTURE.md)** - How the engine works internally
- 🛠️ **[DEVELOPMENT.md](DEVELOPMENT.md)** - Advanced development guide
- 💻 **[example/ExampleGame.cpp](example/ExampleGame.cpp)** - Working code example

### 4️⃣ **Ready to Create?** → Choose Your Path

#### Path A: C++ Game Development
1. Read QUICKSTART.md sections 1-4
2. Create your game in C++
3. Link against TitanEngine library
4. Extend with custom components/systems

#### Path B: Scripting with Lua
1. Create a Lua script in `mods/`
2. Implement `OnModInit()`, `OnUpdate()`, `OnModCleanup()`
3. Load mod with `scripting.LoadMod("path/to/mod.lua")`
4. See `mods/example_mod.lua` for examples

#### Path C: Hybrid Approach
1. Use C++ for core game logic
2. Use Lua for game balancing and mods
3. Expose custom C++ APIs to Lua

## 📂 File Guide

### Core Engine
```
include/Core.hpp          → Entity, Component, Transform, RigidBody
include/Engine.hpp        → Main engine class
include/Renderer.hpp      → Graphics system
include/Input.hpp         → Keyboard/mouse input
include/Physics.hpp       → Physics simulation
include/Scripting.hpp     → Lua integration
include/Audio.hpp         → Sound management
include/Window.hpp        → Window creation
```

### Implementation
```
src/*.cpp                 → Implementation of header files
```

### Examples
```
example/ExampleGame.*     → Complete working game
mods/example_mod.lua      → Example Lua mod
```

### Documentation
```
README.md                 → Feature overview & API docs
QUICKSTART.md             → Quick setup & examples
DEVELOPMENT.md            → Advanced guide & patterns
ARCHITECTURE.md           → System design & internals
PROJECT_SUMMARY.md        → Complete project overview
```

## 🎮 Quick Examples

### Create an Entity with Components
```cpp
auto& entityMgr = engine.GetEntityManager();
Titan::EntityID player = entityMgr.CreateEntity("Player");
auto entity = entityMgr.GetEntity(player);

auto transform = std::make_shared<Titan::Transform>(glm::vec3(0, 1, 0));
entity->AddComponent<Titan::Transform>(transform);

auto rigidbody = std::make_shared<Titan::RigidBody>();
entity->AddComponent<Titan::RigidBody>(rigidbody);
```

### Handle Input
```cpp
auto& input = dynamic_cast<Titan::SimpleInputSystem&>(engine.GetInputSystem());

if (input.IsKeyPressed(Titan::KeyCode::W)) {
    // Move forward
}

if (input.IsMouseButtonPressed(Titan::MouseButton::Left)) {
    // Fire weapon
}
```

### Load a Lua Mod
```cpp
auto& scripting = engine.GetScriptingSystem();
scripting.LoadMod("mods/my_mod.lua");
```

### Write a Lua Mod
```lua
function OnModInit()
    Print("Mod loaded!")
end

function OnUpdate(deltaTime)
    Print("Frame: " .. deltaTime)
end

function OnModCleanup()
    Print("Mod unloaded!")
end
```

## 📊 Engine Capabilities

| Feature | Status | Details |
|---------|--------|---------|
| **ECS System** | ✅ Complete | Entity-Component architecture |
| **Event Bus** | ✅ Complete | Publish/Subscribe communication |
| **Rendering** | ✅ Complete | OpenGL with material system |
| **Physics** | ✅ Complete | Rigid body simulation |
| **Input** | ✅ Complete | Keyboard & mouse support |
| **Scripting** | ✅ Complete | Lua integration with hot-reload |
| **Audio** | ✅ Complete | Audio clip management |
| **Windows** | ✅ Complete | Window & OpenGL context |
| **Example Game** | ✅ Complete | Working demo project |
| **Documentation** | ✅ Complete | 1,200+ lines of guides |
| **Tools** | 🔄 Placeholder | For future extensions |

## 🔧 Key Concepts

### Entity-Component-System (ECS)
- **Entity**: Container for data (components)
- **Component**: Pure data holder (no logic)
- **System**: Processes entities with specific components

### Event-Driven
Systems communicate through events instead of direct references, enabling loose coupling and extensibility.

### Modular Systems
Each system (Rendering, Physics, Input, Audio, Scripting) is independent and can be extended or replaced.

## 🚀 Build & Run

### Build
```powershell
cd TitanEngine
.\build.bat
```

### Run Example Game
```powershell
.\build\Release\TitanGame.exe
```

### Create New Project
1. Copy `example/` folder as template
2. Modify `ExampleGame.cpp` for your game
3. Update `CMakeLists.txt` to reference TitanEngine library
4. Build and run!

## 🤔 Common Questions

**Q: How do I create custom game components?**
A: See DEVELOPMENT.md section "Component Development"

**Q: Can I use this engine commercially?**
A: Yes! Titan Engine is available for educational and commercial use.

**Q: How do I add physics bodies to entities?**
A: See QUICKSTART.md section "Add Physics"

**Q: Can I integrate a different physics engine?**
A: Yes! PhysicsSystem is an interface that can be implemented with Bullet Physics, PhysX, etc.

**Q: How do I optimize for performance?**
A: See DEVELOPMENT.md section "Performance Optimization"

**Q: Can I extend the engine with new systems?**
A: Yes! Inherit from ISystem and register in Engine::InitializeSystems()

## 📚 Documentation Structure

```
PROJECT_SUMMARY.md       (What's included, overview)
    ↓
QUICKSTART.md           (5-minute setup, basic usage)
    ↓
README.md               (Complete feature docs, API reference)
    ↓
DEVELOPMENT.md          (Advanced patterns, custom systems)
    ↓
ARCHITECTURE.md         (Internal design, system architecture)
```

## 🎯 Recommended Learning Path

### Beginner
1. Read PROJECT_SUMMARY.md (5 min)
2. Follow QUICKSTART.md (10 min)
3. Run example game (2 min)
4. Review example code (15 min)

### Intermediate
1. Read README.md entirely (20 min)
2. Review ARCHITECTURE.md (15 min)
3. Modify ExampleGame.cpp (30 min)
4. Create custom component (30 min)

### Advanced
1. Read DEVELOPMENT.md thoroughly (30 min)
2. Create custom system (45 min)
3. Integrate Bullet Physics (1 hour)
4. Build complete game (ongoing)

## 🔗 Quick Links

- [Project Summary](PROJECT_SUMMARY.md)
- [Quick Start Guide](QUICKSTART.md)
- [README](README.md)
- [Architecture Docs](ARCHITECTURE.md)
- [Development Guide](DEVELOPMENT.md)
- [Example Game](example/ExampleGame.cpp)
- [Example Mod](mods/example_mod.lua)

## 📞 Support

For issues or questions:
1. Check relevant documentation (README, DEVELOPMENT, ARCHITECTURE)
2. Review example code and mods
3. Check inline code comments
4. Refer to design patterns in DEVELOPMENT.md

## 🎉 Let's Build Something Amazing!

Titan Engine is ready for your next project. Whether you're making a simple 2D game, a complex 3D experience, or a modded experience, this engine provides the foundation.

**Happy game developing!** 🎮

---

**Last Updated:** December 4, 2025
**Engine Version:** 1.0.0
**Status:** Production-Ready ✅
