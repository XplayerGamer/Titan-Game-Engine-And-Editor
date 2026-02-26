#pragma once

#include "Core.hpp"
#include <string>
#include <memory>

namespace Titan {

// ============================================================================
// Physics System Interface
// ============================================================================

class PhysicsSystem : public ISystem {
public:
    virtual ~PhysicsSystem() = default;

    virtual void Initialize() override = 0;
    virtual void Update(float deltaTime) override = 0;
    virtual void Shutdown() override = 0;

    virtual void SetGravity(const glm::vec3& gravity) = 0;
    virtual glm::vec3 GetGravity() const = 0;

    virtual void AddRigidBody(EntityID entityID, const std::shared_ptr<RigidBody>& body) = 0;
    virtual void RemoveRigidBody(EntityID entityID) = 0;

    virtual void Raycast(const glm::vec3& origin, const glm::vec3& direction, 
                        float maxDistance, std::vector<EntityID>& outHits) = 0;
};

// ============================================================================
// Simple Physics System Implementation
// ============================================================================

class SimplePhysicsSystem : public PhysicsSystem {
private:
    glm::vec3 gravity{0.0f, -9.81f, 0.0f};
    std::unordered_map<EntityID, std::shared_ptr<RigidBody>> rigidBodies;

public:
    void Initialize() override;
    void Update(float deltaTime) override;
    void Shutdown() override;

    void SetGravity(const glm::vec3& g) override { gravity = g; }
    glm::vec3 GetGravity() const override { return gravity; }

    void AddRigidBody(EntityID entityID, const std::shared_ptr<RigidBody>& body) override;
    void RemoveRigidBody(EntityID entityID) override;

    void Raycast(const glm::vec3& origin, const glm::vec3& direction,
                float maxDistance, std::vector<EntityID>& outHits) override;

private:
    void UpdateRigidBody(EntityID entityID, float dt);
};

} // namespace Titan
