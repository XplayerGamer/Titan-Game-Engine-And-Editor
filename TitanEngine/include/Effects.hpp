#pragma once

#include "Core.hpp"
#include "Renderer.hpp"
#include <memory>
#include <vector>
#include <glm/glm.hpp>

namespace Titan {

// ============================================================================
// Particle System
// ============================================================================

struct Particle {
    glm::vec3 position;
    glm::vec3 velocity;
    glm::vec4 color;
    float lifetime;
    float maxLifetime;
    float size;
};

class ParticleEmitter : public Component {
public:
    std::vector<Particle> particles;
    
    // Emission
    float emissionRate{100.0f};
    float emissionAccumulator{0.0f};
    
    // Particle properties
    glm::vec3 velocityMin{-1, -1, -1};
    glm::vec3 velocityMax{1, 1, 1};
    float lifetimeMin{1.0f};
    float lifetimeMax{3.0f};
    glm::vec4 colorStart{1, 1, 1, 1};
    glm::vec4 colorEnd{1, 1, 1, 0};
    float sizeStart{1.0f};
    float sizeEnd{0.1f};

    ParticleEmitter() = default;

    static constexpr ComponentID StaticID() { return 53; }
    ComponentID GetComponentID() const override { return StaticID(); }

    void Update(float deltaTime);
    void Emit(const glm::vec3& position, int count);
};

// ============================================================================
// Decal System
// ============================================================================

struct Decal {
    glm::vec3 position;
    glm::vec3 normal;
    float lifetime;
    float maxLifetime;
    std::string texturePath;
};

class DecalSystem : public ISystem {
private:
    std::vector<Decal> decals;
    static constexpr size_t MAX_DECALS = 1000;

public:
    void Initialize() override;
    void Update(float deltaTime) override;
    void Shutdown() override;

    void SpawnDecal(const glm::vec3& pos, const glm::vec3& normal, 
                    const std::string& texture, float lifetime = 5.0f);

    const auto& GetDecals() const { return decals; }
};

// ============================================================================
// Lighting System
// ============================================================================

enum class LightType {
    Directional,
    Point,
    Spot,
};

struct Light {
    glm::vec3 position;
    glm::vec3 direction;
    glm::vec4 color;
    float intensity;
    float range;
    float angle;  // For spot lights
    LightType type;
};

class LightComponent : public Component {
public:
    Light light;
    bool castShadows{true};

    LightComponent() = default;
    explicit LightComponent(const Light& l) : light(l) {}

    static constexpr ComponentID StaticID() { return 54; }
    ComponentID GetComponentID() const override { return StaticID(); }
};

// ============================================================================
// Advanced Renderer with Effects
// ============================================================================

class AdvancedRenderer : public Renderer {
protected:
    std::vector<Light> lights;
    uint32_t shadowMapFBO{0};
    uint32_t shadowMapTexture{0};
    
    ParticleEmitter* activeParticleEmitter{nullptr};
    std::vector<Decal> decals;

public:
    virtual void RenderParticles(const ParticleEmitter& emitter) = 0;
    virtual void RenderDecals(const std::vector<Decal>& decals) = 0;
    virtual void AddLight(const Light& light) = 0;
    virtual void RemoveLight(const Light& light) = 0;
    virtual void UpdateLighting() = 0;
};

// ============================================================================
// Enhanced OpenGL Renderer
// ============================================================================

class EnhancedGLRenderer : public AdvancedRenderer {
private:
    uint32_t particleVAO{0};
    uint32_t particleVBO{0};
    uint32_t decalVAO{0};

public:
    void Initialize() override;
    void Update(float deltaTime) override;
    void Shutdown() override;

    void BeginFrame() override;
    void EndFrame() override;
    void Present() override;

    void SubmitMesh(const Mesh& mesh, const glm::mat4& transform) override;
    void SetClearColor(const glm::vec4& color) override;
    void SetViewMatrix(const glm::mat4& view) override;
    void SetProjectionMatrix(const glm::mat4& projection) override;

    uint32_t LoadTexture(const std::string& path) override;
    void UnloadTexture(uint32_t textureID) override;

    void DrawDebugLine(const glm::vec3& from, const glm::vec3& to, const glm::vec4& color) override;
    void DrawDebugSphere(const glm::vec3& center, float radius, const glm::vec4& color) override;

    // Advanced features
    void RenderParticles(const ParticleEmitter& emitter) override;
    void RenderDecals(const std::vector<Decal>& decals) override;
    void AddLight(const Light& light) override;
    void RemoveLight(const Light& light) override;
    void UpdateLighting() override;

private:
    void RenderShadowPass();
    void ApplyPostProcessing();
};

} // namespace Titan
