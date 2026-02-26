#include "../include/TitanEditor.hpp"
#include "../include/Engine.hpp"
#include "../include/Audio.hpp"
#include "../include/Window.hpp"
#include "../include/Physics.hpp"
#include "../include/Input.hpp"
#include "../include/Scripting.hpp"
#include "../include/Renderer.hpp"
#include <iostream>
#include <fstream>
#include <algorithm>

namespace Titan {

bool TitanEditor::Initialize(const std::string& projectPath_) {
    projectPath = projectPath_;
    previewEngine = new Engine();
    auto eng = static_cast<Engine*>(previewEngine);
    EngineConfig cfg;
    cfg.appName = "TitanEditorPreview";
    cfg.windowWidth = 1280;
    cfg.windowHeight = 720;
    cfg.vsync = true;
    cfg.targetFPS = 60;
    if (!eng->Initialize(cfg)) {
        std::cerr << "Failed to initialize preview engine" << std::endl;
        delete eng;
        previewEngine = nullptr;
        return false;
    }
    // Preload minimal assets for the editor demo
    assets.AddMesh("builtin/cube.obj");
    assets.AddMesh("builtin/plane.obj");
    assets.AddMaterial("builtin/default.mat");
    assets.AddTexture("builtin/white.png");
    return true;
}

void TitanEditor::Shutdown() {
    if (previewEngine) {
        auto eng = static_cast<Engine*>(previewEngine);
        eng->Shutdown();
        delete eng;
        previewEngine = nullptr;
    }
}

void TitanEditor::ShutdownPreview() {
    if (previewEngine) {
        auto eng = static_cast<Engine*>(previewEngine);
        eng->Shutdown();
        delete eng;
        previewEngine = nullptr;
    }
}

void TitanEditor::Run() {
    if (!previewEngine) return;
    running = true;
    // If ImGui / UI is unavailable, run a simple CLI REPL for editing the scene.
    std::cout << "Titan Editor (CLI mode). Type 'help' for commands." << std::endl;
    std::string cmd;
    while (running) {
        std::cout << "> ";
        if (!std::getline(std::cin, cmd)) break;
        if (cmd.empty()) continue;
        if (cmd == "quit" || cmd == "exit") { running = false; break; }
        if (cmd == "help") { PrintHelp(); continue; }
        if (cmd.rfind("create ", 0) == 0) {
            std::string name = cmd.substr(7);
            uint32_t id = CreateEntity(name.empty() ? "Entity" : name);
            std::cout << "Created entity " << id << " ('" << name << "')" << std::endl;
            continue;
        }
        if (cmd.rfind("list", 0) == 0) {
            for (auto &p : entities) {
                const auto &e = p.second;
                std::cout << e.id << ": " << e.name << " pos(" << e.position.x << "," << e.position.y << "," << e.position.z << ")" << std::endl;
            }
            continue;
        }
        if (cmd.rfind("save ", 0) == 0) {
            std::string path = cmd.substr(5);
            if (SaveMap(path)) std::cout << "Saved map to " << path << std::endl; else std::cout << "Failed to save map" << std::endl;
            continue;
        }
        if (cmd.rfind("load ", 0) == 0) {
            std::string path = cmd.substr(5);
            if (LoadMap(path)) std::cout << "Loaded map " << path << std::endl; else std::cout << "Failed to load map" << std::endl;
            continue;
        }
        if (cmd.rfind("select ", 0) == 0) {
            try {
                uint32_t id = static_cast<uint32_t>(std::stoul(cmd.substr(7)));
                auto *e = GetEntity(id);
                if (e) std::cout << "Selected " << e->id << " ('" << e->name << "')" << std::endl; else std::cout << "Entity not found" << std::endl;
            } catch (...) { std::cout << "Invalid id" << std::endl; }
            continue;
        }
        std::cout << "Unknown command: " << cmd << std::endl;
    }
}

bool TitanEditor::NewMap(const std::string& name) { (void)name; entities.clear(); nextEntityID = 1; return true; }
bool TitanEditor::LoadMap(const std::string& path) { return LoadMapText(path); }
bool TitanEditor::SaveMap(const std::string& path) { return SaveMapText(path); }

uint32_t TitanEditor::CreateEntity(const std::string& name) {
    EditorEntity e;
    e.id = nextEntityID++;
    if (name.empty()) e.name = std::string("Entity") + std::to_string(e.id); else e.name = name;
    entities[e.id] = e;
    return e.id;
}

EditorEntity* TitanEditor::GetEntity(uint32_t id) {
    auto it = entities.find(id);
    return (it != entities.end()) ? &it->second : nullptr;
}

bool TitanEditor::RemoveEntity(uint32_t id) {
    return entities.erase(id) > 0;
}

void TitanEditor::PrintHelp() {
    std::cout << "Commands:\n"
              << "  help               - show this help\n"
              << "  create <name>      - create an entity\n"
              << "  list               - list entities\n"
              << "  select <id>        - select entity by id\n"
              << "  save <path>        - save map to path\n"
              << "  load <path>        - load map from path\n"
              << "  quit/exit          - exit editor\n";
}

// Simple text map format: one entity per line
bool TitanEditor::SaveMapText(const std::string& path) {
    std::ofstream f(path);
    if (!f.is_open()) return false;
    for (auto &p : entities) {
        const auto &e = p.second;
        f << e.id << " " << e.name << " "
          << e.position.x << " " << e.position.y << " " << e.position.z << " "
          << e.rotation.x << " " << e.rotation.y << " " << e.rotation.z << " "
          << e.scale.x << " " << e.scale.y << " " << e.scale.z << " "
          << e.meshPath << " " << e.materialPath << "\n";
    }
    f.close();
    return true;
}

bool TitanEditor::LoadMapText(const std::string& path) {
    std::ifstream f(path);
    if (!f.is_open()) return false;
    entities.clear();
    nextEntityID = 1;
    while (!f.eof()) {
        EditorEntity e;
        if (!(f >> e.id)) break;
        if (!(f >> e.name)) break;
        f >> e.position.x >> e.position.y >> e.position.z;
        f >> e.rotation.x >> e.rotation.y >> e.rotation.z;
        f >> e.scale.x >> e.scale.y >> e.scale.z;
        f >> e.meshPath >> e.materialPath;
        entities[e.id] = e;
        nextEntityID = std::max(nextEntityID, e.id + 1u);
    }
    f.close();
    return true;
}

} // namespace Titan
