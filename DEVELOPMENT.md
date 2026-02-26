# Titan Engine - Development Guide

## Table of Contents
1. [Architecture Overview](#architecture-overview)
2. [Core Concepts](#core-concepts)
3. [System Development](#system-development)
4. [Component Development](#component-development)
5. [Scripting Integration](#scripting-integration)
6. [Performance Optimization](#performance-optimization)
7. [Debugging](#debugging)

## Architecture Overview

Titan Engine uses a **hybrid ECS (Entity-Component-System) + event-driven architecture** designed for flexibility and extensibility.

```
┌─────────────────────────────────────────┐
│          Engine Main Loop               │
├─────────────────────────────────────────┤
│                                         │
│  1. Input System      (Poll user input) │
│  2. Physics System    (Simulate bodies) │
│  3. Scripting System  (Run Lua scripts) │
│  4. Audio System      (Play sounds)     │
│  5. Renderer          (Draw frame)      │
│                                         │
└─────────────────────────────────────────┘
         ↓
    Event Bus
    (Decouples systems)
         ↓
┌─────────────────────────────────────────┐
│       Entity Manager (ECS Core)         │
├─────────────────────────────────────────┤
│                                         │
│  Entities ─→ Components                 │
│   ID 1  ──→ Transform                   │
│         ──→ RigidBody                   │
│         ──→ Renderable                  │
│   ID 2  ──→ Transform                   │
│         ──→ AudioSource                 │
│                                         │
└─────────────────────────────────────────┘
```

## Core Concepts

### Entity
A container for components. Pure data holder with no logic.

```cpp
// Creating an entity
Titan::EntityID entityID = entityManager.CreateEntity("Player");
auto entity = entityManager.GetEntity(entityID);
```

### Component
Data containers attached to entities. Components hold state only.

```cpp
class Transform : public Titan::Component {
public:
    glm::vec3 position{0.0f};
    glm::vec3 rotation{0.0f};
    glm::vec3 scale{1.0f};
    
    static constexpr Titan::ComponentID StaticID() { return 1; }
    Titan::ComponentID GetComponentID() const override { return StaticID(); }
};
```

### System
Processes entities with specific components. Contains all game logic.

```cpp
class PhysicsSystem : public Titan::ISystem {
public:
    void Update(float deltaTime) override {
        // Iterate over entities with RigidBody component
        // Apply physics simulation
    }
};
```

### Event
Decouples system communication. Systems publish/subscribe to events.

```cpp
struct EntityDestroyedEvent : public Titan::Event {
    Titan::EntityID entityID;
    EntityDestroyedEvent(Titan::EntityID id) 
        : Event(1), entityID(id) {}
};

eventBus.Publish(EntityDestroyedEvent(playerID));
```

## System Development

### Creating a Custom System

```cpp
#include <Titan/Core.hpp>

class ParticleSystem : public Titan::ISystem {
private:
    std::vector<Particle> particles;
    
public:
    void Initialize() override {
        std::cout << "Particle system initialized" << std::endl;
        // Setup resources
    }
    
    void Update(float deltaTime) override {
        // Update all particles
        for (auto& particle : particles) {
            particle.position += particle.velocity * deltaTime;
            particle.lifetime -= deltaTime;
        }
        
        // Remove dead particles
        particles.erase(
            std::remove_if(particles.begin(), particles.end(),
                [](const Particle& p) { return p.lifetime <= 0.0f; }),
            particles.end()
        );
    }
    
    void Shutdown() override {
        particles.clear();
    }
    
    void SpawnParticle(const glm::vec3& position, const glm::vec3& velocity) {
        particles.push_back({position, velocity, 2.0f});
    }
};
```

### Registering a System with the Engine

In `Engine.cpp`, modify `InitializeSystems()`:

```cpp
void Engine::InitializeSystems() {
    renderer->Initialize();
    inputSystem->Initialize();
    scriptingSystem->Initialize();
    physicsSystem->Initialize();
    audioSystem->Initialize();
    
    // Add your system
    particleSystem = std::make_unique<ParticleSystem>();
    particleSystem->Initialize();
    
    systems.push_back(inputSystem.get());
    systems.push_back(physicsSystem.get());
    systems.push_back(scriptingSystem.get());
    systems.push_back(audioSystem.get());
    systems.push_back(particleSystem.get());  // Add here
    systems.push_back(renderer.get());
}
```

## Component Development

### Creating Custom Components

```cpp
// Health component for damage system
class HealthComponent : public Titan::Component {
public:
    float currentHealth{100.0f};
    float maxHealth{100.0f};
    bool isDead{false};
    
    static constexpr Titan::ComponentID StaticID() { return 100; }
    Titan::ComponentID GetComponentID() const override { return StaticID(); }
    
    bool TakeDamage(float damage) {
        currentHealth -= damage;
        if (currentHealth <= 0.0f) {
            currentHealth = 0.0f;
            isDead = true;
            return true;  // Died this frame
        }
        return false;
    }
    
    void Heal(float amount) {
        currentHealth = glm::min(currentHealth + amount, maxHealth);
    }
    
    float GetHealthPercent() const {
        return currentHealth / maxHealth;
    }
};

// Weapon component
class WeaponComponent : public Titan::Component {
public:
    float damage{25.0f};
    float fireRate{0.1f};
    float ammo{30.0f};
    float maxAmmo{30.0f};
    float timeSinceLastShot{0.0f};
    
    static constexpr Titan::ComponentID StaticID() { return 101; }
    Titan::ComponentID GetComponentID() const override { return StaticID(); }
    
    bool CanFire() const {
        return ammo > 0.0f && timeSinceLastShot >= fireRate;
    }
    
    void Fire() {
        if (CanFire()) {
            ammo -= 1.0f;
            timeSinceLastShot = 0.0f;
        }
    }
    
    void Reload() {
        ammo = maxAmmo;
    }
};
```

### Using Custom Components

```cpp
auto& entityManager = engine.GetEntityManager();
auto entity = entityManager.GetEntity(playerID);

// Add components
auto health = std::make_shared<HealthComponent>();
entity->AddComponent<HealthComponent>(health);

auto weapon = std::make_shared<WeaponComponent>();
entity->AddComponent<WeaponComponent>(weapon);

// Access components
auto healthPtr = entity->GetComponent<HealthComponent>();
if (healthPtr) {
    healthPtr->TakeDamage(10.0f);
    std::cout << "Health: " << healthPtr->GetHealthPercent() * 100 << "%" << std::endl;
}

auto weaponPtr = entity->GetComponent<WeaponComponent>();
if (weaponPtr && weaponPtr->CanFire()) {
    weaponPtr->Fire();
}
```

## Scripting Integration

### Exposing C++ Functions to Lua

In `Scripting.cpp`, add your functions to `RegisterEngineAPI()`:

```cpp
void LuaScriptingSystem::RegisterEngineAPI() {
    // Get damage function
    lua_register(luaState, "TakeDamage", [](lua_State* L) -> int {
        Titan::EntityID entityID = static_cast<Titan::EntityID>(lua_tonumber(L, 1));
        float damage = static_cast<float>(lua_tonumber(L, 2));
        
        auto& engine = GetEngine();
        auto entity = engine.GetEntityManager().GetEntity(entityID);
        if (entity) {
            auto health = entity->GetComponent<HealthComponent>();
            if (health) {
                bool died = health->TakeDamage(damage);
                lua_pushboolean(L, died);
                return 1;
            }
        }
        lua_pushboolean(L, false);
        return 1;
    });
    
    lua_register(luaState, "GetHealth", [](lua_State* L) -> int {
        Titan::EntityID entityID = static_cast<Titan::EntityID>(lua_tonumber(L, 1));
        
        auto& engine = GetEngine();
        auto entity = engine.GetEntityManager().GetEntity(entityID);
        if (entity) {
            auto health = entity->GetComponent<HealthComponent>();
            if (health) {
                lua_pushnumber(L, health->GetHealthPercent() * 100.0);
                return 1;
            }
        }
        lua_pushnumber(L, 0.0);
        return 1;
    });
}
```

### Using Lua Mods

Create `mods/health_system.lua`:

```lua
-- Health system mod

local HealthSystem = {
    entities = {},
    damageMultiplier = 1.0
}

function OnModInit()
    Print("Health System Mod Loaded")
    Print("Health multiplier: " .. HealthSystem.damageMultiplier)
end

function OnUpdate(deltaTime)
    -- This is where you'd process health updates from Lua
    -- Example: Check if player died
    -- if GetHealth(playerID) <= 0 then
    --     Print("Player is dead!")
    -- end
end

function DealDamage(entityID, baseDamage)
    local damage = baseDamage * HealthSystem.damageMultiplier
    local died = TakeDamage(entityID, damage)
    
    Print("Dealt " .. damage .. " damage to entity " .. entityID)
    
    if died then
        Print("Entity " .. entityID .. " has been defeated!")
        return true
    end
    
    return false
end

function OnModCleanup()
    Print("Health System Mod Unloaded")
end

Print("Health Mod Script Loaded")
```

## Performance Optimization

### Entity Pool for Frequent Allocations

```cpp
class EntityPool {
private:
    std::vector<std::shared_ptr<Titan::Entity>> pool;
    std::queue<std::shared_ptr<Titan::Entity>> available;
    
public:
    std::shared_ptr<Titan::Entity> GetEntity(const std::string& name) {
        if (available.empty()) {
            pool.push_back(std::make_shared<Titan::Entity>());
            return pool.back();
        }
        
        auto entity = available.front();
        available.pop();
        entity->SetName(name);
        return entity;
    }
    
    void ReturnEntity(std::shared_ptr<Titan::Entity> entity) {
        // Clear components
        available.push(entity);
    }
};
```

### Spatial Partitioning for Physics

```cpp
class SpatialGrid {
private:
    static const int GRID_SIZE = 10;
    std::vector<std::vector<Titan::EntityID>> grid;
    
public:
    void UpdateEntity(Titan::EntityID id, const glm::vec3& position) {
        int x = static_cast<int>(position.x) / GRID_SIZE;
        int y = static_cast<int>(position.z) / GRID_SIZE;
        
        if (x >= 0 && x < 100 && y >= 0 && y < 100) {
            grid[x * 100 + y].push_back(id);
        }
    }
    
    std::vector<Titan::EntityID> GetNearby(const glm::vec3& pos, float radius) {
        // Return only entities within grid cells near position
        std::vector<Titan::EntityID> result;
        // Implementation...
        return result;
    }
};
```

## Debugging

### Debug Visualization

```cpp
// Draw entity bounds
auto& renderer = engine.GetRenderer();
auto entity = entityManager.GetEntity(entityID);
auto transform = entity->GetComponent<Titan::Transform>();

if (transform) {
    // Draw position
    renderer.DrawDebugSphere(transform->position, 0.5f, glm::vec4(1, 0, 0, 1));
    
    // Draw forward direction
    glm::vec3 forward = transform->position + transform->GetForward() * 2.0f;
    renderer.DrawDebugLine(transform->position, forward, glm::vec4(0, 1, 0, 1));
    
    // Draw right direction
    glm::vec3 right = transform->position + transform->GetRight() * 2.0f;
    renderer.DrawDebugLine(transform->position, right, glm::vec4(1, 1, 0, 1));
}
```

### Logging System Extension

```cpp
class LogSystem : public Titan::ISystem {
private:
    std::vector<std::string> logs;
    
public:
    void Log(const std::string& message) {
        logs.push_back("[" + std::to_string(GetEngine().GetElapsedTime()) + "] " + message);
    }
    
    void Update(float deltaTime) override {
        // Could send logs to file or display them
    }
    
    void ExportLogs(const std::string& filename) {
        std::ofstream file(filename);
        for (const auto& log : logs) {
            file << log << "\n";
        }
    }
};
```

## Best Practices

1. **Use Components for Data**: Never store logic in components, only data
2. **Keep Systems Focused**: One system should handle one responsibility
3. **Event-Driven Communication**: Use events instead of direct references
4. **Pool Resources**: Reuse allocated objects when possible
5. **Profile Before Optimizing**: Use actual measurements to guide optimization
6. **Document Custom Components**: Include component ID and usage examples
7. **Test Mods Independently**: Test Lua mods in isolation before integration

## Common Patterns

### Observer Pattern (via Events)
```cpp
// Subject publishes events, observers subscribe
eventBus.Subscribe(ENEMY_DIED_EVENT, [](const Event& e) {
    // Update score, play sound, etc.
});
```

### Factory Pattern
```cpp
class EntityFactory {
public:
    static Titan::EntityID CreatePlayer(const glm::vec3& pos) {
        auto id = entityManager.CreateEntity("Player");
        auto entity = entityManager.GetEntity(id);
        // Setup components...
        return id;
    }
};
```

### Singleton Pattern (Engine)
```cpp
Titan::Engine& engine = Titan::GetEngine();  // Get global instance
```

---

For questions or contributions, refer to the README.md and example projects.
