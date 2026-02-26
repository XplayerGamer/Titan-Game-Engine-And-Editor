#include "../include/Core.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/euler_angles.hpp>

namespace Titan {

// ============================================================================
// Transform Implementation
// ============================================================================

glm::mat4 Transform::GetModelMatrix() const {
    glm::mat4 model = glm::identity<glm::mat4>();
    
    // Translate
    model = glm::translate(model, position);
    
    // Rotate (using Euler angles ZYX order)
    model *= glm::eulerAngleZYX(rotation.z, rotation.y, rotation.x);
    
    // Scale
    model = glm::scale(model, scale);
    
    return model;
}

glm::vec3 Transform::GetForward() const {
    return glm::normalize(glm::vec3(
        std::sin(rotation.y),
        0.0f,
        std::cos(rotation.y)
    ));
}

glm::vec3 Transform::GetRight() const {
    return glm::normalize(glm::vec3(
        std::cos(rotation.y),
        0.0f,
        -std::sin(rotation.y)
    ));
}

glm::vec3 Transform::GetUp() const {
    return glm::vec3(0.0f, 1.0f, 0.0f);
}

// ============================================================================
// RigidBody Implementation
// ============================================================================

void RigidBody::ApplyForce(const glm::vec3& force) {
    if (mass > 0.0f && !isKinematic) {
        acceleration += force / mass;
    }
}

void RigidBody::SetVelocity(const glm::vec3& vel) {
    velocity = vel;
}

// ============================================================================
// AudioSource Implementation
// ============================================================================

void AudioSource::Play() {
    playing = true;
}

void AudioSource::Stop() {
    playing = false;
}

void AudioSource::SetVolume(float v) {
    volume = glm::clamp(v, 0.0f, 1.0f);
}

// ============================================================================
// EntityManager Implementation
// ============================================================================

EntityID EntityManager::CreateEntity(const std::string& name) {
    auto entity = std::make_shared<Entity>(nextID, name);
    entities[nextID] = entity;
    return nextID++;
}

void EntityManager::DestroyEntity(EntityID id) {
    entities.erase(id);
}

std::shared_ptr<Entity> EntityManager::GetEntity(EntityID id) {
    auto it = entities.find(id);
    return (it != entities.end()) ? it->second : nullptr;
}

void EntityManager::Clear() {
    entities.clear();
    nextID = 1;
}

// ============================================================================
// EventBus Implementation
// ============================================================================

void EventBus::Subscribe(EventID eventType, std::function<void(const Event&)> callback) {
    subscribers[eventType].push_back(callback);
}

void EventBus::Publish(const Event& event) {
    auto it = subscribers.find(event.eventType);
    if (it != subscribers.end()) {
        for (auto& callback : it->second) {
            callback(event);
        }
    }
}

void EventBus::Clear() {
    subscribers.clear();
}

} // namespace Titan
