#include "ExampleGame.hpp"
#include "Input.hpp"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

bool ExampleGame::Initialize() {
    std::cout << "=== Titan Engine - Example Game ===" << std::endl;

    Titan::EngineConfig config;
    config.appName = "Titan Engine - Example Game";
    config.windowWidth = 1280;
    config.windowHeight = 720;
    config.targetFPS = 60;
    config.vsync = true;

    if (!engine.Initialize(config)) {
        std::cerr << "Failed to initialize engine!" << std::endl;
        return false;
    }

    SetupScene();
    return true;
}

void ExampleGame::SetupScene() {
    auto& entityManager = engine.GetEntityManager();
    auto& eventBus = engine.GetEventBus();

    // Create player entity
    playerEntity = entityManager.CreateEntity("Player");
    auto playerTransform = std::make_shared<Titan::Transform>(glm::vec3(0.0f, 0.0f, 5.0f));
    auto playerEntity_ptr = entityManager.GetEntity(playerEntity);
    playerEntity_ptr->AddComponent<Titan::Transform>(playerTransform);

    // Add physics to player
    auto playerRigidBody = std::make_shared<Titan::RigidBody>();
    playerRigidBody->mass = 1.0f;
    playerRigidBody->useGravity = false;
    playerEntity_ptr->AddComponent<Titan::RigidBody>(playerRigidBody);
    engine.GetPhysicsSystem().AddRigidBody(playerEntity, playerRigidBody);

    // Create cube entity
    cubeEntity = entityManager.CreateEntity("Cube");
    auto cubeTransform = std::make_shared<Titan::Transform>(glm::vec3(0.0f, 0.0f, 0.0f));
    auto cubeEntity_ptr = entityManager.GetEntity(cubeEntity);
    cubeEntity_ptr->AddComponent<Titan::Transform>(cubeTransform);

    // Add renderable to cube
    auto cubeRenderable = std::make_shared<Titan::Renderable>("assets/cube.mesh", "assets/default.mat");
    cubeEntity_ptr->AddComponent<Titan::Renderable>(cubeRenderable);

    // Add physics to cube
    auto cubeRigidBody = std::make_shared<Titan::RigidBody>();
    cubeRigidBody->mass = 2.0f;
    cubeEntity_ptr->AddComponent<Titan::RigidBody>(cubeRigidBody);
    engine.GetPhysicsSystem().AddRigidBody(cubeEntity, cubeRigidBody);

    std::cout << "Scene setup complete!" << std::endl;
    std::cout << "  - Player entity created at (0, 0, 5)" << std::endl;
    std::cout << "  - Cube entity created at (0, 0, 0)" << std::endl;
}

void ExampleGame::HandleInput() {
    auto& inputSystem = dynamic_cast<Titan::SimpleInputSystem&>(engine.GetInputSystem());
    auto playerEntity_ptr = engine.GetEntityManager().GetEntity(playerEntity);
    if (!playerEntity_ptr) return;

    auto transform = playerEntity_ptr->GetComponent<Titan::Transform>();
    if (!transform) return;

    const float moveSpeed = 5.0f;

    // WASD movement
    if (inputSystem.IsKeyPressed(Titan::KeyCode::W)) {
        transform->position += transform->GetForward() * moveSpeed * engine.GetDeltaTime();
    }
    if (inputSystem.IsKeyPressed(Titan::KeyCode::S)) {
        transform->position -= transform->GetForward() * moveSpeed * engine.GetDeltaTime();
    }
    if (inputSystem.IsKeyPressed(Titan::KeyCode::A)) {
        transform->position -= transform->GetRight() * moveSpeed * engine.GetDeltaTime();
    }
    if (inputSystem.IsKeyPressed(Titan::KeyCode::D)) {
        transform->position += transform->GetRight() * moveSpeed * engine.GetDeltaTime();
    }

    // Space to jump
    if (inputSystem.IsKeyPressed(Titan::KeyCode::Space)) {
        auto rigidBody = playerEntity_ptr->GetComponent<Titan::RigidBody>();
        if (rigidBody) {
            rigidBody->velocity.y = 5.0f;
        }
    }

    // ESC to close
    if (inputSystem.IsKeyPressed(Titan::KeyCode::Escape)) {
        engine.Stop();
    }
}

void ExampleGame::UpdateGame(float deltaTime) {
    auto cubeEntity_ptr = engine.GetEntityManager().GetEntity(cubeEntity);
    if (!cubeEntity_ptr) return;

    auto transform = cubeEntity_ptr->GetComponent<Titan::Transform>();
    if (!transform) return;

    // Rotate cube
    transform->rotation.y += rotationSpeed * deltaTime;
}

void ExampleGame::Run() {
    std::cout << "Starting game loop..." << std::endl;
    std::cout << "Controls: WASD = Move, Space = Jump, ESC = Exit" << std::endl;

    while (true) {
        HandleInput();
        UpdateGame(engine.GetDeltaTime());

        // The engine's main loop is handled here
        engine.Run();
        if (!engine.GetEntityManager().GetEntity(playerEntity)) {
            break;  // Exit if player entity is destroyed
        }
    }
}

void ExampleGame::Shutdown() {
    engine.Shutdown();
    std::cout << "Game shutdown complete" << std::endl;
}
