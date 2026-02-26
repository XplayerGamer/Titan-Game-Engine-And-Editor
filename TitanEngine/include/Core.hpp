#pragma once

#include <cstdint>
#include <string>
#include <memory>
#include <vector>
#include <unordered_map>
#include <functional>
#include <queue>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>
#include "TitanExports.hpp"

namespace Titan {

// ============================================================================
// Type Definitions
// ============================================================================

using EntityID = uint32_t;
using ComponentID = uint32_t;
using EventID = uint32_t;
using invalid_entity = std::integral_constant<EntityID, 0>;

// ============================================================================
// Base Classes
// ============================================================================

class Component {
public:
    virtual ~Component() = default;
    virtual ComponentID GetComponentID() const = 0;
};

class ISystem {
public:
    virtual ~ISystem() = default;
    virtual void Initialize() {}
    virtual void Update(float deltaTime) = 0;
    virtual void Shutdown() {}
};

class Event {
public:
    EventID eventType;
    
    explicit Event(EventID type) : eventType(type) {}
    virtual ~Event() = default;
};

// ============================================================================
// Transform Component
// ============================================================================

class Transform : public Component {
public:
    glm::vec3 position{0.0f};
    glm::vec3 rotation{0.0f};  // Euler angles in radians
    glm::vec3 scale{1.0f};

    Transform() = default;
    explicit Transform(const glm::vec3& pos) : position(pos) {}

    static constexpr ComponentID StaticID() { return 1; }
    ComponentID GetComponentID() const override { return StaticID(); }

    glm::mat4 GetModelMatrix() const;
    glm::vec3 GetForward() const;
    glm::vec3 GetRight() const;
    glm::vec3 GetUp() const;
};

// ============================================================================
// Physics Component (Placeholder for future physics engine integration)
// ============================================================================

class RigidBody : public Component {
public:
    glm::vec3 velocity{0.0f};
    glm::vec3 acceleration{0.0f};
    float mass{1.0f};
    bool useGravity{true};
    bool isKinematic{false};

    RigidBody() = default;

    static constexpr ComponentID StaticID() { return 2; }
    ComponentID GetComponentID() const override { return StaticID(); }

    void ApplyForce(const glm::vec3& force);
    void SetVelocity(const glm::vec3& vel);
};

// ============================================================================
// Renderable Component
// ============================================================================

class Renderable : public Component {
public:
    std::string meshPath;
    std::string materialPath;
    bool visible{true};
    uint32_t renderLayer{0};

    Renderable() = default;
    explicit Renderable(const std::string& mesh, const std::string& material)
        : meshPath(mesh), materialPath(material) {}

    static constexpr ComponentID StaticID() { return 3; }
    ComponentID GetComponentID() const override { return StaticID(); }
};

// ============================================================================
// Audio Source Component
// ============================================================================

class AudioSource : public Component {
public:
    std::string audioPath;
    float volume{1.0f};
    bool loop{false};
    bool playing{false};

    AudioSource() = default;
    explicit AudioSource(const std::string& path) : audioPath(path) {}

    static constexpr ComponentID StaticID() { return 4; }
    ComponentID GetComponentID() const override { return StaticID(); }

    void Play();
    void Stop();
    void SetVolume(float v);
};

// ============================================================================
// Entity
// ============================================================================

class Entity {
    friend class EntityManager;

private:
    EntityID id{invalid_entity::value};
    std::string name;
    std::unordered_map<ComponentID, std::shared_ptr<Component>> components;
    bool active{true};

public:
    Entity() = default;
    explicit Entity(EntityID entityID, const std::string& entityName)
        : id(entityID), name(entityName) {}

    EntityID GetID() const { return id; }
    const std::string& GetName() const { return name; }
    void SetName(const std::string& newName) { name = newName; }
    bool IsActive() const { return active; }
    void SetActive(bool state) { active = state; }

    template<typename T>
    void AddComponent(std::shared_ptr<T> component) {
        static_assert(std::is_base_of_v<Component, T>, "T must inherit from Component");
        components[T::StaticID()] = component;
    }

    template<typename T>
    std::shared_ptr<T> GetComponent() {
        static_assert(std::is_base_of_v<Component, T>, "T must inherit from Component");
        auto it = components.find(T::StaticID());
        return (it != components.end()) ? std::static_pointer_cast<T>(it->second) : nullptr;
    }

    template<typename T>
    bool HasComponent() const {
        static_assert(std::is_base_of_v<Component, T>, "T must inherit from Component");
        return components.find(T::StaticID()) != components.end();
    }

    template<typename T>
    void RemoveComponent() {
        static_assert(std::is_base_of_v<Component, T>, "T must inherit from Component");
        components.erase(T::StaticID());
    }

    const auto& GetAllComponents() const { return components; }
};

// ============================================================================
// Entity Manager
// ============================================================================

class TITAN_API EntityManager {
private:
    std::unordered_map<EntityID, std::shared_ptr<Entity>> entities;
    EntityID nextID{1};

public:
    EntityID CreateEntity(const std::string& name = "Entity");
    void DestroyEntity(EntityID id);
    std::shared_ptr<Entity> GetEntity(EntityID id);
    const auto& GetAllEntities() const { return entities; }
    void Clear();
};

// ============================================================================
// Event System
// ============================================================================

class EventBus {
private:
    std::unordered_map<EventID, std::vector<std::function<void(const Event&)>>> subscribers;

public:
    void Subscribe(EventID eventType, std::function<void(const Event&)> callback);
    void Publish(const Event& event);
    void Clear();
};

// ============================================================================
// Engine Configuration
// ============================================================================

struct EngineConfig {
    std::string appName{"Titan Engine"};
    uint32_t windowWidth{1280};
    uint32_t windowHeight{720};
    uint32_t targetFPS{60};
    bool vsync{true};
    bool headless{false};  // Useful for dedicated servers or batch processing
};

} // namespace Titan
