#include "../include/Performance.hpp"
#include <algorithm>
#include <iostream>
#include <cmath>
#include <chrono>

namespace Titan {

// ============================================================================
// AABB Implementation
// ============================================================================

bool AABB::Intersects(const AABB& other) const {
    return !(max.x < other.min.x || min.x > other.max.x ||
             max.y < other.min.y || min.y > other.max.y ||
             max.z < other.min.z || min.z > other.max.z);
}

bool AABB::Contains(const glm::vec3& point) const {
    return point.x >= min.x && point.x <= max.x &&
           point.y >= min.y && point.y <= max.y &&
           point.z >= min.z && point.z <= max.z;
}

// ============================================================================
// Sphere Implementation
// ============================================================================

bool Sphere::Intersects(const AABB& aabb) const {
    float sqDist = 0.0f;

    for (int i = 0; i < 3; ++i) {
        float v = center[i];
        if (v < aabb.min[i]) sqDist += (aabb.min[i] - v) * (aabb.min[i] - v);
        else if (v > aabb.max[i]) sqDist += (v - aabb.max[i]) * (v - aabb.max[i]);
    }

    return sqDist <= radius * radius;
}

bool Sphere::Intersects(const Sphere& other) const {
    float dist = glm::distance(center, other.center);
    return dist <= (radius + other.radius);
}

// ============================================================================
// Frustum Implementation
// ============================================================================

bool Frustum::Contains(const glm::vec3& point) const {
    for (int i = 0; i < 6; ++i) {
        if (glm::dot(planes[i], glm::vec4(point, 1.0f)) < 0.0f) {
            return false;
        }
    }
    return true;
}

bool Frustum::Contains(const Sphere& sphere) const {
    for (int i = 0; i < 6; ++i) {
        float dist = glm::dot(planes[i], glm::vec4(sphere.center, 1.0f));
        if (dist < -sphere.radius) {
            return false;
        }
    }
    return true;
}

bool Frustum::Contains(const AABB& aabb) const {
    for (int i = 0; i < 6; ++i) {
        glm::vec3 p = aabb.min;
        if (planes[i].x >= 0) p.x = aabb.max.x;
        if (planes[i].y >= 0) p.y = aabb.max.y;
        if (planes[i].z >= 0) p.z = aabb.max.z;

        if (glm::dot(planes[i], glm::vec4(p, 1.0f)) < 0.0f) {
            return false;
        }
    }
    return true;
}

// ============================================================================
// Spatial Hash Implementation
// ============================================================================

uint32_t SpatialHash::GetCellKey(const glm::vec3& position) const {
    int32_t x = static_cast<int32_t>(position.x / cellSize);
    int32_t y = static_cast<int32_t>(position.y / cellSize);
    int32_t z = static_cast<int32_t>(position.z / cellSize);

    // Simple hash function for 3D coordinates
    return ((x * 73856093) ^ (y * 19349663) ^ (z * 83492791));
}

void SpatialHash::Insert(EntityID id, const glm::vec3& position) {
    uint32_t key = GetCellKey(position);
    grid[key].entities.push_back(id);
}

void SpatialHash::Update(EntityID id, const glm::vec3& oldPos, const glm::vec3& newPos) {
    uint32_t oldKey = GetCellKey(oldPos);
    uint32_t newKey = GetCellKey(newPos);

    if (oldKey != newKey) {
        auto& oldCell = grid[oldKey];
        auto it = std::find(oldCell.entities.begin(), oldCell.entities.end(), id);
        if (it != oldCell.entities.end()) {
            oldCell.entities.erase(it);
        }

        grid[newKey].entities.push_back(id);
    }
}

void SpatialHash::Remove(EntityID id, const glm::vec3& position) {
    uint32_t key = GetCellKey(position);
    auto& cell = grid[key];
    auto it = std::find(cell.entities.begin(), cell.entities.end(), id);
    if (it != cell.entities.end()) {
        cell.entities.erase(it);
    }
}

std::vector<EntityID> SpatialHash::QuerySphere(const glm::vec3& center, float radius) const {
    std::vector<EntityID> result;
    Sphere sphere(center, radius);

    int32_t minX = static_cast<int32_t>((center.x - radius) / cellSize);
    int32_t maxX = static_cast<int32_t>((center.x + radius) / cellSize);
    int32_t minY = static_cast<int32_t>((center.y - radius) / cellSize);
    int32_t maxY = static_cast<int32_t>((center.y + radius) / cellSize);
    int32_t minZ = static_cast<int32_t>((center.z - radius) / cellSize);
    int32_t maxZ = static_cast<int32_t>((center.z + radius) / cellSize);

    for (int32_t x = minX; x <= maxX; ++x) {
        for (int32_t y = minY; y <= maxY; ++y) {
            for (int32_t z = minZ; z <= maxZ; ++z) {
                uint32_t key = ((x * 73856093) ^ (y * 19349663) ^ (z * 83492791));
                auto it = grid.find(key);
                if (it != grid.end()) {
                    for (EntityID id : it->second.entities) {
                        result.push_back(id);
                    }
                }
            }
        }
    }

    return result;
}

std::vector<EntityID> SpatialHash::QueryAABB(const AABB& aabb) const {
    std::vector<EntityID> result;

    int32_t minX = static_cast<int32_t>(aabb.min.x / cellSize);
    int32_t maxX = static_cast<int32_t>(aabb.max.x / cellSize);
    int32_t minY = static_cast<int32_t>(aabb.min.y / cellSize);
    int32_t maxY = static_cast<int32_t>(aabb.max.y / cellSize);
    int32_t minZ = static_cast<int32_t>(aabb.min.z / cellSize);
    int32_t maxZ = static_cast<int32_t>(aabb.max.z / cellSize);

    for (int32_t x = minX; x <= maxX; ++x) {
        for (int32_t y = minY; y <= maxY; ++y) {
            for (int32_t z = minZ; z <= maxZ; ++z) {
                uint32_t key = ((x * 73856093) ^ (y * 19349663) ^ (z * 83492791));
                auto it = grid.find(key);
                if (it != grid.end()) {
                    for (EntityID id : it->second.entities) {
                        result.push_back(id);
                    }
                }
            }
        }
    }

    return result;
}

void SpatialHash::Clear() {
    grid.clear();
}

// ============================================================================
// Culling System Implementation
// ============================================================================

void CullingSystem::Initialize() {
    std::cout << "Culling system initialized" << std::endl;
}

void CullingSystem::Update(float deltaTime) {
    // Frustum culling is performed during rendering
}

void CullingSystem::Shutdown() {
    std::cout << "Culling system shutdown" << std::endl;
}

void CullingSystem::UpdateViewFrustum(const glm::mat4& viewProj) {
    // Extract frustum planes from view-projection matrix
    // This is a simplified version - full implementation would extract all 6 planes
}

void CullingSystem::RegisterEntity(EntityID id) {
    allEntities.insert(id);
}

void CullingSystem::UnregisterEntity(EntityID id) {
    allEntities.erase(id);
    visibleEntities.erase(id);
}

bool CullingSystem::IsEntityVisible(EntityID id) const {
    return visibleEntities.find(id) != visibleEntities.end();
}

// ============================================================================
// Performance Monitor Implementation
// ============================================================================

void PerformanceMonitor::StartFrame() {
    frameStartTime = std::chrono::high_resolution_clock::now().time_since_epoch().count() / 1e9;
}

void PerformanceMonitor::EndFrame() {
    if (frameHistory.size() >= maxHistory) {
        frameHistory.erase(frameHistory.begin());
    }
}

void PerformanceMonitor::RecordRenderTime(float time) {
    if (!frameHistory.empty()) {
        frameHistory.back().renderTime = time;
    }
}

void PerformanceMonitor::RecordPhysicsTime(float time) {
    if (!frameHistory.empty()) {
        frameHistory.back().physicsTime = time;
    }
}

void PerformanceMonitor::RecordScriptTime(float time) {
    if (!frameHistory.empty()) {
        frameHistory.back().scriptTime = time;
    }
}

void PerformanceMonitor::RecordEntityCount(uint32_t count) {
    if (!frameHistory.empty()) {
        frameHistory.back().entityCount = count;
    }
}

void PerformanceMonitor::RecordRenderedEntities(uint32_t count) {
    if (!frameHistory.empty()) {
        frameHistory.back().renderedEntities = count;
    }
}

float PerformanceMonitor::GetAverageFPS() const {
    if (frameHistory.empty()) return 0.0f;

    float avgDelta = GetAverageDeltaTime();
    return avgDelta > 0.0f ? 1.0f / avgDelta : 0.0f;
}

float PerformanceMonitor::GetAverageDeltaTime() const {
    if (frameHistory.empty()) return 0.0f;

    float sum = 0.0f;
    for (const auto& frame : frameHistory) {
        sum += frame.deltaTime;
    }
    return sum / frameHistory.size();
}

float PerformanceMonitor::GetAverageRenderTime() const {
    if (frameHistory.empty()) return 0.0f;

    float sum = 0.0f;
    for (const auto& frame : frameHistory) {
        sum += frame.renderTime;
    }
    return sum / frameHistory.size();
}

} // namespace Titan
