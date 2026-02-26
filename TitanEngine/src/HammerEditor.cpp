#include "../include/HammerEditor.hpp"
#include "../include/Engine.hpp"
#include "../include/Audio.hpp"
#include "../include/Window.hpp"
#include "../include/Physics.hpp"
#include "../include/Input.hpp"
#include "../include/Scripting.hpp"
#include "../include/Renderer.hpp"
#include <iostream>

namespace Titan {

bool HammerEditor::Initialize(const std::string& projectPath_) {
    projectPath = projectPath_;
    // Create an engine instance for live preview (using default config)
    previewEngine = std::make_unique<Engine>();
    EngineConfig cfg;
    cfg.appName = "HammerPreview";
    cfg.windowWidth = 1280;
    cfg.windowHeight = 720;
    cfg.vsync = true;
    cfg.targetFPS = 60;
    if (!previewEngine->Initialize(cfg)) {
        std::cerr << "Failed to initialize preview engine" << std::endl;
        previewEngine.reset();
        return false;
    }
    return true;
}

void HammerEditor::Shutdown() {
    if (previewEngine) {
        previewEngine->Shutdown();
        previewEngine.reset();
    }
}

void HammerEditor::Run() {
    if (!previewEngine) return;
    running = true;
    while (running && previewEngine->GetRenderer().IsReady()) {
        previewEngine->Run();
        // For an editor we'd normally run a different loop, render UI overlays, etc.
        running = false; // for scaffold, run only once
    }
}

bool HammerEditor::NewMap(const std::string& name) {
    // TODO: implement map creation
    (void)name;
    return true;
}

bool HammerEditor::LoadMap(const std::string& path) {
    (void)path;
    // TODO: implement map load and spawn entities in previewEngine
    return true;
}

bool HammerEditor::SaveMap(const std::string& path) {
    (void)path;
    // TODO: serialize current scene
    return true;
}

} // namespace Titan
