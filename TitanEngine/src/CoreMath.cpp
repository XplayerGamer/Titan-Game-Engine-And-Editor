#include "../include/CoreMath.hpp"
#include <cmath>

namespace Titan {

std::optional<BallisticSolution> SolveBallisticArc(const glm::vec3& origin, const glm::vec3& target, float speed, float gravity) {
    glm::vec3 diff = target - origin;
    glm::vec3 diffXZ = glm::vec3(diff.x, 0.0f, diff.z);
    float x = glm::length(diffXZ);
    float y = diff.y;

    if (x < 1e-6f) return std::nullopt;

    float v2 = speed * speed;
    float g = gravity;

    float inside = v2 * v2 - g * (g * x * x + 2.0f * y * v2);
    if (inside < 0.0f) return std::nullopt;

    float sqrtInside = std::sqrt(inside);

    // Two possible angles (high and low). We'll pick the lower-angle (smaller theta).
    float theta1 = std::atan2(v2 - sqrtInside, g * x);
    float theta2 = std::atan2(v2 + sqrtInside, g * x);
    float theta = std::min(theta1, theta2);

    float vxz = speed * std::cos(theta);
    float vy = speed * std::sin(theta);

    glm::vec3 dirXZ = glm::normalize(diffXZ);
    glm::vec3 initialVelocity = dirXZ * vxz + glm::vec3(0.0f, vy, 0.0f);

    // time of flight approximation
    float time = x / vxz;

    return BallisticSolution{initialVelocity, time};
}

bool RayIntersectsAABB(const glm::vec3& rayOrigin, const glm::vec3& rayDir, const glm::vec3& aabbMin, const glm::vec3& aabbMax, float& outT) {
    float tmin = (aabbMin.x - rayOrigin.x) / rayDir.x;
    float tmax = (aabbMax.x - rayOrigin.x) / rayDir.x;
    if (tmin > tmax) std::swap(tmin, tmax);

    float tymin = (aabbMin.y - rayOrigin.y) / rayDir.y;
    float tymax = (aabbMax.y - rayOrigin.y) / rayDir.y;
    if (tymin > tymax) std::swap(tymin, tymax);

    if ((tmin > tymax) || (tymin > tmax)) return false;
    if (tymin > tmin) tmin = tymin;
    if (tymax < tmax) tmax = tymax;

    float tzmin = (aabbMin.z - rayOrigin.z) / rayDir.z;
    float tzmax = (aabbMax.z - rayOrigin.z) / rayDir.z;
    if (tzmin > tzmax) std::swap(tzmin, tzmax);

    if ((tmin > tzmax) || (tzmin > tmax)) return false;
    if (tzmin > tmin) tmin = tzmin;
    if (tzmax < tmax) tmax = tzmax;

    outT = tmin;
    return true;
}

} // namespace Titan
