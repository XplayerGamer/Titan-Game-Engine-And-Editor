#pragma once

#include "Core.hpp"
#include <memory>
#include <vector>
#include <unordered_set>

namespace Titan {

// ============================================================================
// Optimization Types
// ============================================================================

struct AABB {
    glm::vec3 min;
    glm::vec3 max;

    AABB() : min(0), max(0) {}
    AABB(const glm::vec3& minVal, const glm::vec3& maxVal) : min(minVal), max(maxVal) {}

    bool Intersects(const AABB& other) const;
    bool Contains(const glm::vec3& point) const;
};

struct Sphere {
    glm::vec3 center;
    float radius;

    Sphere() : center(0), radius(0) {}
    Sphere(const glm::vec3& c, float r) : center(c), radius(r) {}

    bool Intersects(const AABB& aabb) const;
    bool Intersects(const Sphere& other) const;
};

struct Frustum {
    glm::vec4 planes[6];  // Left, Right, Top, Bottom, Near, Far

    bool Contains(const glm::vec3& point) const;
    bool Contains(const Sphere& sphere) const;
    bool Contains(const AABB& aabb) const;
};

// ============================================================================
// Spatial Acceleration Structure
// ============================================================================

class SpatialHash {
public:
    struct GridCell {
        std::vector<EntityID> entities;
    };

private:
    float cellSize{50.0f};
    std::unordered_map<uint32_t, GridCell> grid;

    uint32_t GetCellKey(const glm::vec3& position) const;

public:
    explicit SpatialHash(float size) : cellSize(size) {}

    void Insert(EntityID id, const glm::vec3& position);
    void Update(EntityID id, const glm::vec3& oldPos, const glm::vec3& newPos);
    void Remove(EntityID id, const glm::vec3& position);

    std::vector<EntityID> QuerySphere(const glm::vec3& center, float radius) const;
    std::vector<EntityID> QueryAABB(const AABB& aabb) const;

    void Clear();
};

// ============================================================================
// Culling System
// ============================================================================

class CullingSystem : public ISystem {
private:
    Frustum viewFrustum;
    std::unordered_set<EntityID> visibleEntities;
    std::unordered_set<EntityID> allEntities;

public:
    void Initialize() override;
    void Update(float deltaTime) override;
    void Shutdown() override;

    void UpdateViewFrustum(const glm::mat4& viewProj);
    void RegisterEntity(EntityID id);
    void UnregisterEntity(EntityID id);

    const auto& GetVisibleEntities() const { return visibleEntities; }
    bool IsEntityVisible(EntityID id) const;
};

// ============================================================================
// Object Pool
// ============================================================================

template<typename T>
class ObjectPool {
private:
    std::vector<T> pool;
    std::vector<bool> active;
    size_t nextAvailable{0};

public:
    explicit ObjectPool(size_t initialSize) : pool(initialSize), active(initialSize, false) {}

    template<typename... Args>
    T* Acquire(Args&&... args) {
        if (nextAvailable < pool.size()) {
            active[nextAvailable] = true;
            pool[nextAvailable] = T(std::forward<Args>(args)...);
            return &pool[nextAvailable++];
        }
        return nullptr;
    }

    void Release(T* obj) {
        if (obj >= pool.data() && obj < pool.data() + pool.size()) {
            size_t index = obj - pool.data();
            active[index] = false;
            if (index < nextAvailable) {
                nextAvailable = index;
            }
        }
    }

    void Clear() {
        std::fill(active.begin(), active.end(), false);
        nextAvailable = 0;
    }

    size_t GetActiveCount() const {
        return std::count(active.begin(), active.end(), true);
    }
};

// ============================================================================
// Performance Monitor
// ============================================================================

class PerformanceMonitor {
private:
    struct FrameStats {
        float deltaTime{0.0f};
        float renderTime{0.0f};
        float physicsTime{0.0f};
        float scriptTime{0.0f};
        uint32_t entityCount{0};
        uint32_t renderedEntities{0};
    };

    std::vector<FrameStats> frameHistory;
    size_t maxHistory{300};  // 5 seconds at 60 FPS
    double frameStartTime{0.0};

public:
    void Initialize() {}
    void Shutdown() {}
    void StartFrame();
    void EndFrame();
    void RecordRenderTime(float time);
    void RecordPhysicsTime(float time);
    void RecordScriptTime(float time);
    void RecordEntityCount(uint32_t count);
    void RecordRenderedEntities(uint32_t count);

    float GetAverageFPS() const;
    float GetAverageDeltaTime() const;
    float GetAverageRenderTime() const;
    const auto& GetFrameHistory() const { return frameHistory; }
};

} // namespace Titan
