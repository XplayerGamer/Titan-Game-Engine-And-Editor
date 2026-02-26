#pragma once

#include <string>
#include <memory>
#include "Engine.hpp"

namespace Titan {

// HammerEditor scaffold - standalone editor application that uses the
// engine runtime to preview maps and entities. This is a minimal scaffold
// that can be extended with ImGui, filesystem watchers, and map tools.

class HammerEditor {
public:
    HammerEditor() = default;
    ~HammerEditor() = default;

    // Initialize editor with path to project root
    bool Initialize(const std::string& projectPath);
    void Shutdown();

    // Main editor loop (blocking)
    void Run();

    // Map operations
    bool NewMap(const std::string& name);
    bool LoadMap(const std::string& path);
    bool SaveMap(const std::string& path);

private:
    std::string projectPath;
    std::unique_ptr<Engine> previewEngine; // optional runtime preview
    bool running{false};
};

} // namespace Titan
