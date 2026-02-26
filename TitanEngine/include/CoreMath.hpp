#pragma once

#include <glm/glm.hpp>
#include <optional>

namespace Titan {

// Utility math helpers focused on projectile and trace calculations used by
// weapons, physics, and gameplay systems.

struct BallisticSolution {
    glm::vec3 initialVelocity;
    float timeOfFlight;
};

// Solve for initial velocity to hit target from origin with projectile speed and gravity.
// Returns optional solution (first found) — prefers lower-angle solution.
// origin: source position
// target: target position
// speed: muzzle speed
// gravity: positive-down vector magnitude (e.g., 9.81f)
std::optional<BallisticSolution> SolveBallisticArc(const glm::vec3& origin, const glm::vec3& target, float speed, float gravity);

// Simple raycast helper (no scene) - tests ray vs AABB
bool RayIntersectsAABB(const glm::vec3& rayOrigin, const glm::vec3& rayDir, const glm::vec3& aabbMin, const glm::vec3& aabbMax, float& outT);

} // namespace Titan
