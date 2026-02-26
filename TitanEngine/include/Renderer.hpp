#pragma once

#include <cstdint>
#include <string>
#include <memory>
#include <vector>
#include <glm/glm.hpp>
#include "Core.hpp"

namespace Titan {

// ============================================================================
// Vertex Structure
// ============================================================================

struct Vertex {
    glm::vec3 position{0.0f};
    glm::vec3 normal{0.0f, 1.0f, 0.0f};
    glm::vec2 texCoord{0.0f};
    glm::vec3 tangent{1.0f, 0.0f, 0.0f};
    glm::vec3 bitangent{0.0f, 0.0f, 1.0f};

    Vertex() = default;
    Vertex(const glm::vec3& pos) : position(pos) {}
    Vertex(const glm::vec3& pos, const glm::vec3& norm)
        : position(pos), normal(norm) {}
};

// ============================================================================
// Material System
// ============================================================================

struct MaterialProperties {
    glm::vec4 albedo{1.0f};
    float metallic{0.0f};
    float roughness{0.5f};
    float ao{1.0f};
    uint32_t flags{0};  // For material-specific flags
};

class Material {
private:
    std::string name;
    std::string shaderPath;
    MaterialProperties properties;
    std::vector<std::string> textureSlots;  // Paths to textures

public:
    Material(const std::string& materialName, const std::string& shader)
        : name(materialName), shaderPath(shader) {}

    const std::string& GetName() const { return name; }
    const std::string& GetShaderPath() const { return shaderPath; }
    MaterialProperties& GetProperties() { return properties; }
    const MaterialProperties& GetProperties() const { return properties; }

    void AddTextureSlot(const std::string& texturePath) {
        textureSlots.push_back(texturePath);
    }
    const auto& GetTextureSlots() const { return textureSlots; }
};

// ============================================================================
// Mesh Structure
// ============================================================================

class Mesh {
private:
    std::string name;
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
    std::shared_ptr<Material> material;
    uint32_t gpuVertexBuffer{0};
    uint32_t gpuIndexBuffer{0};
    uint32_t gpuVertexArray{0};
    bool isDirty{true};

public:
    Mesh(const std::string& meshName) : name(meshName) {}

    const std::string& GetName() const { return name; }
    
    void SetVertices(const std::vector<Vertex>& verts) {
        vertices = verts;
        isDirty = true;
    }
    void SetIndices(const std::vector<uint32_t>& inds) {
        indices = inds;
        isDirty = true;
    }
    void SetMaterial(std::shared_ptr<Material> mat) { material = mat; }

    const auto& GetVertices() const { return vertices; }
    const auto& GetIndices() const { return indices; }
    std::shared_ptr<Material> GetMaterial() const { return material; }

    uint32_t GetVertexCount() const { return static_cast<uint32_t>(vertices.size()); }
    uint32_t GetIndexCount() const { return static_cast<uint32_t>(indices.size()); }

    bool IsDirty() const { return isDirty; }
    void MarkClean() { isDirty = false; }

    // GPU resource handles
    uint32_t GetGPUVertexBuffer() const { return gpuVertexBuffer; }
    uint32_t GetGPUIndexBuffer() const { return gpuIndexBuffer; }
    uint32_t GetGPUVertexArray() const { return gpuVertexArray; }
    
    void SetGPUResources(uint32_t vao, uint32_t vbo, uint32_t ebo) {
        gpuVertexArray = vao;
        gpuVertexBuffer = vbo;
        gpuIndexBuffer = ebo;
    }
};

// ============================================================================
// Renderer Interface
// ============================================================================

class Renderer : public ISystem {
public:
    virtual ~Renderer() = default;

    virtual void Initialize() override = 0;
    virtual void Update(float deltaTime) override = 0;
    virtual void Shutdown() override = 0;

    virtual void BeginFrame() = 0;
    virtual void EndFrame() = 0;
    virtual void Present() = 0;

    virtual void SubmitMesh(const Mesh& mesh, const glm::mat4& transform) = 0;
    virtual void SetClearColor(const glm::vec4& color) = 0;
    virtual void SetViewMatrix(const glm::mat4& view) = 0;
    virtual void SetProjectionMatrix(const glm::mat4& projection) = 0;

    virtual uint32_t LoadTexture(const std::string& path) = 0;
    virtual void UnloadTexture(uint32_t textureID) = 0;

    virtual void DrawDebugLine(const glm::vec3& from, const glm::vec3& to, const glm::vec4& color) = 0;
    virtual void DrawDebugSphere(const glm::vec3& center, float radius, const glm::vec4& color) = 0;

    // Indicates whether the renderer is initialized and ready to present frames.
    virtual bool IsReady() const { return true; }
};

// ============================================================================
// Simple OpenGL Renderer Implementation
// ============================================================================

class GLRenderer : public Renderer {
private:
    uint32_t defaultVAO{0};
    glm::mat4 viewMatrix;
    glm::mat4 projectionMatrix;
    glm::vec4 clearColor{0.1f, 0.1f, 0.1f, 1.0f};

public:
    void Initialize() override;
    void Update(float deltaTime) override;
    void Shutdown() override;

    void BeginFrame() override;
    void EndFrame() override;
    void Present() override;

    void SubmitMesh(const Mesh& mesh, const glm::mat4& transform) override;
    void SetClearColor(const glm::vec4& color) override { clearColor = color; }
    void SetViewMatrix(const glm::mat4& view) override { viewMatrix = view; }
    void SetProjectionMatrix(const glm::mat4& projection) override { projectionMatrix = projection; }

    uint32_t LoadTexture(const std::string& path) override;
    void UnloadTexture(uint32_t textureID) override;

    void DrawDebugLine(const glm::vec3& from, const glm::vec3& to, const glm::vec4& color) override;
    void DrawDebugSphere(const glm::vec3& center, float radius, const glm::vec4& color) override;

private:
    uint32_t CreateShaderProgram(const std::string& vertexSource, const std::string& fragmentSource);
};

} // namespace Titan
