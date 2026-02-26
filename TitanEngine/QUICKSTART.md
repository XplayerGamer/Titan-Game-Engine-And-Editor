# Quick Start Guide - Titan Engine

## 5-Minute Setup

### Step 1: Clone/Setup
Your engine is located at: `TitanEngine/`

### Step 2: Build
```powershell
# Windows PowerShell
cd TitanEngine
mkdir build
cd build
cmake ..
cmake --build . --config Release
```

### Step 3: Run Example
```powershell
# Run the example game
.\Release\TitanGame.exe
```

## Creating Your First Game

### 1. Create a New Project
```cpp
// game.cpp
#include "Engine.hpp"
#include <iostream>

int main() {
    try {
        // Create engine instance
        Titan::Engine engine;
        
        // Configure
        Titan::EngineConfig config;
        config.appName = "My First Game";
        config.windowWidth = 1280;
        config.windowHeight = 720;
        
        // Initialize
        if (!engine.Initialize(config)) {
            std::cerr << "Failed to initialize!" << std::endl;
            return 1;
        }
        
        // Create game objects
        auto& entityManager = engine.GetEntityManager();
        Titan::EntityID player = entityManager.CreateEntity("Player");
        
        // Run engine
        engine.Run();
        
        return 0;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}
```

### 2. Add Components
```cpp
// Add position and physics to player
auto player_entity = entityManager.GetEntity(player);

auto transform = std::make_shared<Titan::Transform>(
    glm::vec3(0, 1, 0)  // Starting position
);
player_entity->AddComponent<Titan::Transform>(transform);

auto rigidbody = std::make_shared<Titan::RigidBody>();
rigidbody->mass = 1.0f;
player_entity->AddComponent<Titan::RigidBody>(rigidbody);
engine.GetPhysicsSystem().AddRigidBody(player, rigidbody);

auto renderable = std::make_shared<Titan::Renderable>(
    "assets/player.mesh",
    "assets/default_material.mat"
);
player_entity->AddComponent<Titan::Renderable>(renderable);
```

### 3. Handle Input
```cpp
// In your game loop
auto& input = dynamic_cast<Titan::SimpleInputSystem&>(
    engine.GetInputSystem()
);

auto player_entity = entityManager.GetEntity(player);
auto transform = player_entity->GetComponent<Titan::Transform>();

if (input.IsKeyPressed(Titan::KeyCode::W)) {
    transform->position += glm::vec3(0, 0, -1) * 5.0f * engine.GetDeltaTime();
}
if (input.IsKeyPressed(Titan::KeyCode::S)) {
    transform->position += glm::vec3(0, 0, 1) * 5.0f * engine.GetDeltaTime();
}
if (input.IsKeyPressed(Titan::KeyCode::A)) {
    transform->position += glm::vec3(-1, 0, 0) * 5.0f * engine.GetDeltaTime();
}
if (input.IsKeyPressed(Titan::KeyCode::D)) {
    transform->position += glm::vec3(1, 0, 0) * 5.0f * engine.GetDeltaTime();
}
```

### 4. Add Physics
```cpp
auto& physics = engine.GetPhysicsSystem();

// Set world gravity
physics.SetGravity(glm::vec3(0, -9.81f, 0));

// Add physics bodies to entities
auto body = std::make_shared<Titan::RigidBody>();
body->mass = 2.0f;
body->useGravity = true;
physics.AddRigidBody(entityID, body);

// Apply forces
body->ApplyForce(glm::vec3(0, 50, 0));  // Jump
```

### 5. Add Audio
```cpp
auto& audio = engine.GetAudioSystem();

// Load audio
uint32_t jumpSound = audio.LoadAudio("assets/jump.wav");

// Play when needed
if (input.IsKeyPressed(Titan::KeyCode::Space)) {
    audio.PlayAudio(jumpSound);
}
```

## Creating a Lua Mod

### Step 1: Create Mod File
Create `mods/my_first_mod.lua`:

```lua
-- My First Mod

local State = {
    frameCount = 0
}

function OnModInit()
    Print("My mod is loading!")
end

function OnUpdate(deltaTime)
    State.frameCount = State.frameCount + 1
    
    if State.frameCount % 60 == 0 then
        Print("FPS Update - Frame: " .. State.frameCount)
    end
end

function OnModCleanup()
    Print("My mod is unloading!")
end
```

### Step 2: Load Mod in C++
```cpp
auto& scripting = engine.GetScriptingSystem();
scripting.LoadMod("mods/my_first_mod.lua");
```

## Entity-Component Examples

### Enemy Entity
```cpp
Titan::EntityID enemy = entityManager.CreateEntity("Enemy");
auto enemy_entity = entityManager.GetEntity(enemy);

// Position and appearance
auto enemy_transform = std::make_shared<Titan::Transform>(
    glm::vec3(5, 1, 0)
);
enemy_entity->AddComponent<Titan::Transform>(enemy_transform);

auto enemy_renderable = std::make_shared<Titan::Renderable>(
    "assets/enemy.mesh",
    "assets/enemy_material.mat"
);
enemy_entity->AddComponent<Titan::Renderable>(enemy_renderable);

// Physics
auto enemy_body = std::make_shared<Titan::RigidBody>();
enemy_body->mass = 1.5f;
enemy_body->useGravity = true;
enemy_entity->AddComponent<Titan::RigidBody>(enemy_body);
physics.AddRigidBody(enemy, enemy_body);

// Audio
auto enemy_audio = std::make_shared<Titan::AudioSource>(
    "assets/enemy_sound.wav"
);
enemy_entity->AddComponent<Titan::AudioSource>(enemy_audio);
```

### Environment Object (Non-moving)
```cpp
Titan::EntityID ground = entityManager.CreateEntity("Ground");
auto ground_entity = entityManager.GetEntity(ground);

auto ground_transform = std::make_shared<Titan::Transform>(
    glm::vec3(0, 0, 0)
);
ground_transform->scale = glm::vec3(100, 1, 100);  // Large flat plane
ground_entity->AddComponent<Titan::Transform>(ground_transform);

auto ground_renderable = std::make_shared<Titan::Renderable>(
    "assets/ground.mesh",
    "assets/ground_material.mat"
);
ground_entity->AddComponent<Titan::Renderable>(ground_renderable);

// Kinematic body (not affected by physics)
auto ground_body = std::make_shared<Titan::RigidBody>();
ground_body->isKinematic = true;
ground_entity->AddComponent<Titan::RigidBody>(ground_body);
```

## Testing Your Game

### Build and Run
```powershell
# From project root
cd build
cmake --build . --config Release
.\Release\TitanGame.exe
```

### Common Issues

**"Failed to create window"**
- Make sure you're on Windows (other platforms need platform-specific code)
- Check that OpenGL is available

**"Lua error"**
- Check Lua file syntax
- Make sure Print() function is used correctly
- Check `mods/` folder exists

**"Missing dependencies"**
- Ensure CMake finds all packages
- Check LUA_INCLUDE_DIR and LUA_LIBRARIES in CMakeLists.txt

## Next Steps

1. **Read DEVELOPMENT.md** - Learn about creating custom systems and components
2. **Explore Example Game** - Check `example/ExampleGame.cpp` for advanced patterns
3. **Create Custom Components** - Extend the engine with gameplay-specific components
4. **Write Lua Mods** - Implement game mechanics in Lua for rapid iteration
5. **Build More Complex Scenes** - Create multiple entities and interactions

## Key Files to Understand

```
TitanEngine/
├── include/Core.hpp          ← Entity, Component, Event definitions
├── include/Engine.hpp        ← Main engine class
├── example/ExampleGame.cpp   ← Complete working example
├── mods/example_mod.lua      ← Lua mod example
├── README.md                 ← Full documentation
└── DEVELOPMENT.md            ← Advanced development guide
```

## Tips & Tricks

- **Organize entities by type**: Use meaningful names for easy debugging
- **Use events for loose coupling**: Avoid direct references between systems
- **Test components independently**: Create entities with specific components to test
- **Profile early**: Use timing measurements to find bottlenecks
- **Document custom components**: Add clear comments explaining ID, purpose, and usage

---

Happy game developing! 🎮
