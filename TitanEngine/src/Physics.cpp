#include "../include/Physics.hpp"
#include "../include/Engine.hpp"
#include <iostream>

namespace Titan {

// ============================================================================
// SimplePhysicsSystem Implementation
// ============================================================================

void SimplePhysicsSystem::Initialize() {
    std::cout << "Physics system initialized with gravity: ("
              << gravity.x << ", " << gravity.y << ", " << gravity.z << ")" << std::endl;
}

void SimplePhysicsSystem::Update(float deltaTime) {
    // Update all rigid bodies
    for (auto& [entityID, rigidBody] : rigidBodies) {
        UpdateRigidBody(entityID, deltaTime);
    }
}

void SimplePhysicsSystem::Shutdown() {
    std::cout << "Physics system shutdown" << std::endl;
    rigidBodies.clear();
}

void SimplePhysicsSystem::AddRigidBody(EntityID entityID, const std::shared_ptr<RigidBody>& body) {
    rigidBodies[entityID] = body;
    std::cout << "Rigid body added to entity " << entityID << std::endl;
}

void SimplePhysicsSystem::RemoveRigidBody(EntityID entityID) {
    rigidBodies.erase(entityID);
    std::cout << "Rigid body removed from entity " << entityID << std::endl;
}

void SimplePhysicsSystem::Raycast(const glm::vec3& origin, const glm::vec3& direction,
                                 float maxDistance, std::vector<EntityID>& outHits) {
    // Simple raycast implementation
    // In a real physics engine, this would perform proper raycast testing
    std::cout << "Raycast from (" << origin.x << "," << origin.y << "," << origin.z
              << ") in direction (" << direction.x << "," << direction.y << "," << direction.z
              << ") with max distance " << maxDistance << std::endl;
}

void SimplePhysicsSystem::UpdateRigidBody(EntityID entityID, float dt) {
    auto entity = GetEngine().GetEntityManager().GetEntity(entityID);
    if (!entity) return;

    auto rigidBody = entity->GetComponent<RigidBody>();
    auto transform = entity->GetComponent<Transform>();

    if (!rigidBody || !transform) return;

    if (!rigidBody->isKinematic) {
        // Apply gravity
        if (rigidBody->useGravity) {
            rigidBody->ApplyForce(gravity * rigidBody->mass);
        }

        // Update velocity based on acceleration
        rigidBody->velocity += rigidBody->acceleration * dt;
        rigidBody->acceleration = glm::vec3(0.0f);

        // Apply drag
        rigidBody->velocity *= 0.99f;

        // Update position
        transform->position += rigidBody->velocity * dt;
    }
}

} // namespace Titan
