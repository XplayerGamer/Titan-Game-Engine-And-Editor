#pragma once

#include "Core.hpp"
#include "Networking.hpp"
#include "Gamemodes.hpp"
#include "Performance.hpp"
#include "Audio.hpp"
#include "Scripting.hpp"
#include "Input.hpp"
#include "Physics.hpp"
#include "Renderer.hpp"
#include "Window.hpp"
#include "TitanExports.hpp"
#include <memory>
#include <vector>

namespace Titan {

class Window;
class Renderer;
class InputSystem;
class ScriptingSystem;
class PhysicsSystem;
class AudioSystem;

// ============================================================================
// Main Engine Class
// ============================================================================

class TITAN_API Engine {
private:
    EngineConfig config;
    bool running{false};
    float deltaTime{0.0f};
    float elapsedTime{0.0f};
    
    // Core systems
    std::unique_ptr<EntityManager> entityManager;
    std::unique_ptr<EventBus> eventBus;
    std::unique_ptr<Window> window;
    std::unique_ptr<Renderer> renderer;
    std::unique_ptr<InputSystem> inputSystem;
    std::unique_ptr<ScriptingSystem> scriptingSystem;
    std::unique_ptr<PhysicsSystem> physicsSystem;
    std::unique_ptr<AudioSystem> audioSystem;

    // Systems list for update
    std::vector<ISystem*> systems;

    // Advanced systems
    std::unique_ptr<NetworkManager> networkManager;
    std::unique_ptr<Gamemode> gamemode;
    std::unique_ptr<CullingSystem> cullingSystem;
    std::unique_ptr<PerformanceMonitor> performanceMonitor;

    // Timing
    double lastFrameTime{0.0};

public:
    Engine() = default;
    ~Engine();

    // Initialization and shutdown
    bool Initialize(const EngineConfig& engineConfig);
    void Shutdown();

    // Main loop
    void Run();
    void Stop() { running = false; }

    // Frame timing
    float GetDeltaTime() const { return deltaTime; }
    float GetElapsedTime() const { return elapsedTime; }

    // System access
    EntityManager& GetEntityManager() { return *entityManager; }
    EventBus& GetEventBus() { return *eventBus; }
    Renderer& GetRenderer();
    InputSystem& GetInputSystem();
    ScriptingSystem& GetScriptingSystem();
    PhysicsSystem& GetPhysicsSystem();
    AudioSystem& GetAudioSystem();
    NetworkManager& GetNetworkManager();
    Gamemode& GetGamemode();
    PerformanceMonitor& GetPerformanceMonitor();

    // Configuration
    const EngineConfig& GetConfig() const { return config; }

    // Public methods for C API
    void UpdateSystemsPublic(float dt) { UpdateSystems(dt); }
    void RenderFramePublic() { RenderFrame(); }

    // Initialization check
    bool IsInitialized() const { return entityManager != nullptr; }

private:
    void InitializeSystems();
    void UpdateSystems(float dt);
    void RenderFrame();
    void CalculateDeltaTime();
};

// ============================================================================
// Global Engine Instance
// ============================================================================

Engine& GetEngine();
void SetEngineInstance(Engine* engine);

} // namespace Titan
