#include "../include/Engine.hpp"
#include "../include/Window.hpp"
#include "../include/Renderer.hpp"
#include "../include/Input.hpp"
#include "../include/Physics.hpp"
#include "../include/Scripting.hpp"
#include "../include/Audio.hpp"
#include "../include/Networking.hpp"
#include "../include/Gamemodes.hpp"
#include "../include/Performance.hpp"
#include <windows.h>
#include <iostream>
#include <stdexcept>
#include <chrono>

namespace Titan {

static Engine* g_engine = nullptr;

Engine& GetEngine() {
    if (!g_engine) {
        throw std::runtime_error("Engine not initialized!");
    }
    return *g_engine;
}

void SetEngineInstance(Engine* engine) {
    g_engine = engine;
}

Engine::~Engine() = default;

// ============================================================================
// Engine Implementation
// ============================================================================

bool Engine::Initialize(const EngineConfig& engineConfig) {
    config = engineConfig;
    SetEngineInstance(this);

    try {
        // Create systems
        entityManager = std::make_unique<EntityManager>();
        eventBus = std::make_unique<EventBus>();
        window = std::make_unique<Win32Window>();
        renderer = std::make_unique<GLRenderer>();
        inputSystem = std::make_unique<SimpleInputSystem>();
        scriptingSystem = std::make_unique<LuaScriptingSystem>();
        physicsSystem = std::make_unique<SimplePhysicsSystem>();
        audioSystem = std::make_unique<SimpleAudioSystem>();
        
        // Advanced systems
        networkManager = std::make_unique<SimpleNetworkManager>();
        gamemode = std::make_unique<BombDefusalGamemode>();
        cullingSystem = std::make_unique<CullingSystem>();
        performanceMonitor = std::make_unique<PerformanceMonitor>();

        // Create window (only if not headless)
        if (!config.headless) {
            if (!window->Create(config.appName, config.windowWidth, config.windowHeight)) {
                std::cerr << "Failed to create window!" << std::endl;
                return false;
            }
            window->SetVSync(config.vsync);
        }

        // Initialize all systems
        InitializeSystems();

        running = true;
        lastFrameTime = std::chrono::high_resolution_clock::now().time_since_epoch().count() / 1e9;

        std::cout << "Engine initialized successfully!" << std::endl;
        return true;
    }
    catch (const std::exception& e) {
        std::cerr << "Engine initialization failed: " << e.what() << std::endl;
        return false;
    }
}

void Engine::InitializeSystems() {
    // Initialize renderer only if not headless
    if (!config.headless) {
        renderer->Initialize();
    }
    
    inputSystem->Initialize();
    
    // Initialize scripting system (optional)
    try {
        scriptingSystem->Initialize();
        systems.push_back(scriptingSystem.get());
    }
    catch (const std::exception& e) {
        std::cerr << "Warning: Scripting system failed to initialize: " << e.what() << std::endl;
        std::cerr << "Continuing without scripting support." << std::endl;
    }
    
    physicsSystem->Initialize();
    audioSystem->Initialize();

    // Initialize advanced systems and add to update list
    if (networkManager) networkManager->Initialize();
    if (gamemode) gamemode->Initialize();
    if (cullingSystem) cullingSystem->Initialize();
    if (performanceMonitor) performanceMonitor->Initialize();

    systems.push_back(inputSystem.get());
    systems.push_back(physicsSystem.get());
    systems.push_back(audioSystem.get());
    if (networkManager) systems.push_back(networkManager.get());
    if (gamemode) systems.push_back(gamemode.get());
    if (cullingSystem) systems.push_back(cullingSystem.get());
    
    // Add renderer to systems only if not headless
    if (!config.headless) {
        systems.push_back(renderer.get());
    }
}

void Engine::Run() {
    while (running && (config.headless || window->IsOpen())) {
        CalculateDeltaTime();
        
        if (!config.headless) {
            window->Update();
        }
        UpdateSystems(deltaTime);
        
        if (!config.headless) {
            RenderFrame();
        }

        // Simple FPS limit
        if (config.targetFPS > 0) {
            float targetFrameTime = 1.0f / config.targetFPS;
            if (deltaTime < targetFrameTime) {
                float sleepTime = targetFrameTime - deltaTime;
                Sleep(static_cast<DWORD>(sleepTime * 1000));
            }
        }
    }

    Shutdown();
}

void Engine::CalculateDeltaTime() {
    double currentTime = std::chrono::high_resolution_clock::now().time_since_epoch().count() / 1e9;
    deltaTime = static_cast<float>(currentTime - lastFrameTime);
    elapsedTime += deltaTime;
    lastFrameTime = currentTime;

    // Cap deltaTime to prevent physics issues
    if (deltaTime > 0.033f) {  // Cap at ~30 FPS worth of delta
        deltaTime = 0.033f;
    }
}

void Engine::UpdateSystems(float dt) {
    for (auto system : systems) {
        system->Update(dt);
    }
}

void Engine::RenderFrame() {
    renderer->BeginFrame();
    
    // Render all entities
    for (const auto& [entityID, entity] : entityManager->GetAllEntities()) {
        if (!entity->IsActive()) continue;

        auto transform = entity->GetComponent<Transform>();
        auto renderable = entity->GetComponent<Renderable>();

        if (transform && renderable) {
            // This would render the entity
            // renderer->SubmitMesh(mesh, transform->GetModelMatrix());
        }
    }

    renderer->EndFrame();
    renderer->Present();
}

void Engine::Shutdown() {
    std::cout << "Shutting down engine..." << std::endl;

    if (audioSystem) audioSystem->Shutdown();
    if (scriptingSystem) scriptingSystem->Shutdown();
    if (physicsSystem) physicsSystem->Shutdown();
    if (inputSystem) inputSystem->Shutdown();
    if (renderer) renderer->Shutdown();
    if (cullingSystem) cullingSystem->Shutdown();
    if (gamemode) gamemode->Shutdown();
    if (networkManager) networkManager->Shutdown();
    if (performanceMonitor) performanceMonitor->Shutdown();
    if (window) window->Destroy();

    entityManager->Clear();
    eventBus->Clear();

    running = false;
}

NetworkManager& Engine::GetNetworkManager() {
    if (!networkManager) throw std::runtime_error("Network manager not initialized!");
    return *networkManager;
}

Gamemode& Engine::GetGamemode() {
    if (!gamemode) throw std::runtime_error("Gamemode not initialized!");
    return *gamemode;
}

PerformanceMonitor& Engine::GetPerformanceMonitor() {
    if (!performanceMonitor) throw std::runtime_error("Performance monitor not initialized!");
    return *performanceMonitor;
}

Renderer& Engine::GetRenderer() {
    if (!renderer) throw std::runtime_error("Renderer not initialized!");
    return *renderer;
}

InputSystem& Engine::GetInputSystem() {
    if (!inputSystem) throw std::runtime_error("Input system not initialized!");
    return *inputSystem;
}

ScriptingSystem& Engine::GetScriptingSystem() {
    if (!scriptingSystem) throw std::runtime_error("Scripting system not initialized!");
    return *scriptingSystem;
}

PhysicsSystem& Engine::GetPhysicsSystem() {
    if (!physicsSystem) throw std::runtime_error("Physics system not initialized!");
    return *physicsSystem;
}

AudioSystem& Engine::GetAudioSystem() {
    if (!audioSystem) throw std::runtime_error("Audio system not initialized!");
    return *audioSystem;
}

} // namespace Titan

// ============================================================================
// C-style exported functions for runtime DLL loading
// ============================================================================

extern "C" {

TITAN_API void* CreateEngine() {
    try {
        Titan::Engine* engine = new Titan::Engine();
        Titan::SetEngineInstance(engine);
        return engine;
    }
    catch (...) {
        return nullptr;
    }
}

TITAN_API void DestroyEngine(void* engine) {
    if (engine) {
        Titan::Engine* e = static_cast<Titan::Engine*>(engine);
        Titan::SetEngineInstance(nullptr);
        delete e;
    }
}

TITAN_API bool InitializeEngine(void* engine, const char* appName, int width, int height, int targetFPS, bool vsync, bool headless) {
    if (!engine) return false;
    
    try {
        Titan::Engine* e = static_cast<Titan::Engine*>(engine);
        Titan::EngineConfig config;
        config.appName = appName ? appName : "Titan Engine";
        config.windowWidth = width;
        config.windowHeight = height;
        config.targetFPS = targetFPS;
        config.vsync = vsync;
        config.headless = headless;
        
        return e->Initialize(config);
    }
    catch (...) {
        return false;
    }
}

TITAN_API void ShutdownEngine(void* engine) {
    if (engine) {
        Titan::Engine* e = static_cast<Titan::Engine*>(engine);
        e->Shutdown();
    }
}

TITAN_API void UpdateEngine(void* engine, float deltaTime) {
    if (!engine) return;
    Titan::Engine* e = static_cast<Titan::Engine*>(engine);
    if (!e->IsInitialized()) return;
    e->UpdateSystemsPublic(deltaTime);
}

TITAN_API void RenderFrame(void* engine) {
    if (!engine) return;
    Titan::Engine* e = static_cast<Titan::Engine*>(engine);
    if (!e->IsInitialized()) return;
    e->RenderFramePublic();
}

// Entity Management
TITAN_API int CreateEntity(void* engine) {
    if (!engine) return -1;
    Titan::Engine* e = static_cast<Titan::Engine*>(engine);
    if (!e->IsInitialized()) return -1;
    try {
        return e->GetEntityManager().CreateEntity();
    }
    catch (...) {
        return -1;
    }
}

TITAN_API void DestroyEntity(void* engine, int entityId) {
    if (!engine) return;
    Titan::Engine* e = static_cast<Titan::Engine*>(engine);
    if (!e->IsInitialized()) return;
    e->GetEntityManager().DestroyEntity(static_cast<Titan::EntityID>(entityId));
}

TITAN_API void SetEntityPosition(void* engine, int entityId, float x, float y, float z) {
    if (!engine) return;
    Titan::Engine* e = static_cast<Titan::Engine*>(engine);
    if (!e->IsInitialized()) return;
    auto entity = e->GetEntityManager().GetEntity(static_cast<Titan::EntityID>(entityId));
    if (entity) {
        auto transform = entity->GetComponent<Titan::Transform>();
        if (transform) {
            transform->position = glm::vec3(x, y, z);
        }
    }
}

TITAN_API void GetEntityPosition(void* engine, int entityId, float* x, float* y, float* z) {
    if (!engine || !x || !y || !z) return;
    Titan::Engine* e = static_cast<Titan::Engine*>(engine);
    if (!e->IsInitialized()) return;
    auto entity = e->GetEntityManager().GetEntity(static_cast<Titan::EntityID>(entityId));
    if (entity) {
        auto transform = entity->GetComponent<Titan::Transform>();
        if (transform) {
            *x = transform->position.x;
            *y = transform->position.y;
            *z = transform->position.z;
        }
    }
}

// Camera Control
TITAN_API void SetCameraPosition(void* engine, float x, float y, float z) {
    if (engine) {
        Titan::Engine* e = static_cast<Titan::Engine*>(engine);
        // TODO: Implement camera position setting
    }
}

TITAN_API void SetCameraRotation(void* engine, float yaw, float pitch) {
    if (engine) {
        Titan::Engine* e = static_cast<Titan::Engine*>(engine);
        // TODO: Implement camera rotation setting
    }
}

// Physics
TITAN_API void InitializePhysics(void* engine) {
    if (engine) {
        Titan::Engine* e = static_cast<Titan::Engine*>(engine);
        // Physics is initialized as part of engine initialization
    }
}

TITAN_API void UpdatePhysics(void* engine, float deltaTime) {
    if (!engine) return;
    Titan::Engine* e = static_cast<Titan::Engine*>(engine);
    if (!e->IsInitialized()) return;
    e->GetPhysicsSystem().Update(deltaTime);
}

// Scripting
TITAN_API bool LoadScript(void* engine, const char* scriptPath) {
    if (!engine || !scriptPath) return false;
    Titan::Engine* e = static_cast<Titan::Engine*>(engine);
    if (!e->IsInitialized()) return false;
    try {
        return e->GetScriptingSystem().LoadScript(scriptPath);
    }
    catch (...) {
        return false;
    }
}

TITAN_API bool ExecuteScript(void* engine, const char* scriptContent) {
    if (!engine || !scriptContent) return false;
    Titan::Engine* e = static_cast<Titan::Engine*>(engine);
    if (!e->IsInitialized()) return false;
    try {
        return e->GetScriptingSystem().ExecuteString(scriptContent);
    }
    catch (...) {
        return false;
    }
}

}
