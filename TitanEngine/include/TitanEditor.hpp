#pragma once

#include <string>
#include <memory>
#include <vector>
#include <unordered_map>
#include <iostream>

namespace Titan {

class Engine;

// Minimal math forward declarations
struct Vec3 { float x{0}, y{0}, z{0}; };

struct EditorEntity {
    uint32_t id{0};
    std::string name;
    Vec3 position{0,0,0};
    Vec3 rotation{0,0,0};
    Vec3 scale{1,1,1};
    std::string meshPath;
    std::string materialPath;
};

// Simple Asset manager for editor (file lists)
class AssetManager {
public:
    std::vector<std::string> meshes;
    std::vector<std::string> materials;
    std::vector<std::string> textures;

    void AddMesh(const std::string& p) { meshes.push_back(p); }
    void AddMaterial(const std::string& p) { materials.push_back(p); }
    void AddTexture(const std::string& p) { textures.push_back(p); }
};

// TitanEditor: provides scene graph, asset pipeline hooks, entity tools,
// map serialization, live preview (via Engine), and optional UI integration.
class TitanEditor {
public:
    TitanEditor() = default;
    ~TitanEditor() = default;

    // Initialize the editor and optionally create a preview engine.
    bool Initialize(const std::string& projectPath);
    void Shutdown();
    void ShutdownPreview();

    // Run editor main loop (CLI-based when no UI available)
    void Run();

    // Map operations
    bool NewMap(const std::string& name);
    bool LoadMap(const std::string& path);
    bool SaveMap(const std::string& path);

    // Entity editing
    uint32_t CreateEntity(const std::string& name);
    bool RemoveEntity(uint32_t id);
    EditorEntity* GetEntity(uint32_t id);

    // Asset operations
    AssetManager& GetAssetManager() { return assets; }

    // CLI helpers
    void PrintHelp();

private:
    std::string projectPath;
    void* previewEngine{nullptr};  // Opaque pointer to Engine to avoid forward-decl issues
    bool running{false};

    // Scene data
    std::unordered_map<uint32_t, EditorEntity> entities;
    uint32_t nextEntityID{1};

    AssetManager assets;

    // Internal helpers
    bool SaveMapText(const std::string& path);
    bool LoadMapText(const std::string& path);
};

} // namespace Titan
