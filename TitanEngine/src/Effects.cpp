#include "../include/Effects.hpp"
#if !defined(GLAD_GL)
#include "../include/GLStub.hpp"
#endif
#include <iostream>
#include <glm/glm.hpp>

namespace Titan {

// ============================================================================
// Particle Emitter Implementation
// ============================================================================

void ParticleEmitter::Update(float deltaTime) {
    // Emit new particles
    emissionAccumulator += emissionRate * deltaTime;
    int particlesToEmit = static_cast<int>(emissionAccumulator);
    emissionAccumulator -= particlesToEmit;

    // Update existing particles
    for (auto it = particles.begin(); it != particles.end(); ) {
        it->lifetime -= deltaTime;

        if (it->lifetime <= 0.0f) {
            it = particles.erase(it);
        } else {
            // Update physics
            it->position += it->velocity * deltaTime;
            it->velocity *= 0.99f;  // Damping

            // Update color based on lifetime
            float progress = 1.0f - (it->lifetime / it->maxLifetime);
            it->color = glm::mix(colorStart, colorEnd, progress);

            // Update size based on lifetime
            it->size = glm::mix(sizeStart, sizeEnd, progress);

            ++it;
        }
    }
}

void ParticleEmitter::Emit(const glm::vec3& position, int count) {
    for (int i = 0; i < count; ++i) {
        Particle p;
        p.position = position;
        p.velocity = glm::mix(velocityMin, velocityMax, glm::vec3(
            rand() / float(RAND_MAX),
            rand() / float(RAND_MAX),
            rand() / float(RAND_MAX)
        ));
        p.lifetime = glm::mix(lifetimeMin, lifetimeMax, rand() / float(RAND_MAX));
        p.maxLifetime = p.lifetime;
        p.color = colorStart;
        p.size = sizeStart;

        particles.push_back(p);
    }
}

// ============================================================================
// Decal System Implementation
// ============================================================================

void DecalSystem::Initialize() {
    std::cout << "Decal system initialized" << std::endl;
}

void DecalSystem::Update(float deltaTime) {
    // Update decal lifetimes
    for (auto it = decals.begin(); it != decals.end(); ) {
        it->lifetime -= deltaTime;

        if (it->lifetime <= 0.0f) {
            it = decals.erase(it);
        } else {
            ++it;
        }
    }
}

void DecalSystem::Shutdown() {
    std::cout << "Decal system shutdown" << std::endl;
    decals.clear();
}

void DecalSystem::SpawnDecal(const glm::vec3& pos, const glm::vec3& normal,
                            const std::string& texture, float lifetime) {
    if (decals.size() >= MAX_DECALS) {
        decals.erase(decals.begin());
    }

    Decal decal;
    decal.position = pos;
    decal.normal = glm::normalize(normal);
    decal.lifetime = lifetime;
    decal.maxLifetime = lifetime;
    decal.texturePath = texture;

    decals.push_back(decal);

    std::cout << "Decal spawned at (" << pos.x << "," << pos.y << "," << pos.z << ")" << std::endl;
}

// ============================================================================
// Enhanced OpenGL Renderer Implementation
// ============================================================================

void EnhancedGLRenderer::Initialize() {
    std::cout << "Enhanced OpenGL Renderer initialized" << std::endl;

    // Initialize particle rendering
    glGenVertexArrays(1, &particleVAO);
    glGenBuffers(1, &particleVBO);

    glBindVertexArray(particleVAO);
    glBindBuffer(GL_ARRAY_BUFFER, particleVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Particle) * 10000, nullptr, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)offsetof(Particle, position));
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Initialize shadow mapping
    glGenFramebuffers(1, &shadowMapFBO);
    glGenTextures(1, &shadowMapTexture);

    glBindTexture(GL_TEXTURE_2D, shadowMapTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 2048, 2048, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void EnhancedGLRenderer::Update(float deltaTime) {
    // Update lighting and effects
}

void EnhancedGLRenderer::Shutdown() {
    std::cout << "Enhanced OpenGL Renderer shutdown" << std::endl;

    if (particleVAO) glDeleteVertexArrays(1, &particleVAO);
    if (particleVBO) glDeleteBuffers(1, &particleVBO);
    if (shadowMapTexture) glDeleteTextures(1, &shadowMapTexture);
    if (shadowMapFBO) glDeleteFramebuffers(1, &shadowMapFBO);
}

void EnhancedGLRenderer::BeginFrame() {
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void EnhancedGLRenderer::EndFrame() {
    // Finalize rendering
}

void EnhancedGLRenderer::Present() {
    // Swap buffers
}

void EnhancedGLRenderer::SubmitMesh(const Mesh& mesh, const glm::mat4& transform) {
    // Submit mesh for rendering
}

void EnhancedGLRenderer::SetClearColor(const glm::vec4& color) {
    glClearColor(color.r, color.g, color.b, color.a);
}

void EnhancedGLRenderer::SetViewMatrix(const glm::mat4& view) {
    // Set view matrix
}

void EnhancedGLRenderer::SetProjectionMatrix(const glm::mat4& projection) {
    // Set projection matrix
}

uint32_t EnhancedGLRenderer::LoadTexture(const std::string& path) {
    std::cout << "Loading texture: " << path << std::endl;
    return 1;
}

void EnhancedGLRenderer::UnloadTexture(uint32_t textureID) {
    glDeleteTextures(1, &textureID);
}

void EnhancedGLRenderer::DrawDebugLine(const glm::vec3& from, const glm::vec3& to, const glm::vec4& color) {
    // Draw debug line
}

void EnhancedGLRenderer::DrawDebugSphere(const glm::vec3& center, float radius, const glm::vec4& color) {
    // Draw debug sphere
}

void EnhancedGLRenderer::RenderParticles(const ParticleEmitter& emitter) {
    if (emitter.particles.empty()) return;

    glBindVertexArray(particleVAO);
    glBindBuffer(GL_ARRAY_BUFFER, particleVBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, emitter.particles.size() * sizeof(Particle), emitter.particles.data());

    glDrawArrays(GL_POINTS, 0, static_cast<GLsizei>(emitter.particles.size()));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void EnhancedGLRenderer::RenderDecals(const std::vector<Decal>& decals) {
    // Render all decals
    for (const auto& decal : decals) {
        // Render decal quad at position with normal orientation
    }
}

void EnhancedGLRenderer::AddLight(const Light& light) {
    lights.push_back(light);
}

void EnhancedGLRenderer::RemoveLight(const Light& light) {
    // Remove light from lights list
}

void EnhancedGLRenderer::UpdateLighting() {
    // Update lighting uniforms for current scene
}

void EnhancedGLRenderer::RenderShadowPass() {
    glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFBO);
    glClear(GL_DEPTH_BUFFER_BIT);

    // Render scene from light's perspective

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void EnhancedGLRenderer::ApplyPostProcessing() {
    // Apply any post-processing effects
}

} // namespace Titan
