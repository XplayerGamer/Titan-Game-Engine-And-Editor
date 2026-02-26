#pragma once

#include "../include/Engine.hpp"
#include "../include/Core.hpp"
#include "../include/Renderer.hpp"
#include "../include/Input.hpp"
#include "../include/Physics.hpp"
#include <memory>

class ExampleGame {
private:
    Titan::Engine engine;
    Titan::EntityID playerEntity{0};
    Titan::EntityID cubeEntity{0};
    float rotationSpeed{2.0f};

public:
    ExampleGame() = default;
    ~ExampleGame() = default;

    bool Initialize();
    void Run();
    void Shutdown();

private:
    void SetupScene();
    void HandleInput();
    void UpdateGame(float deltaTime);
};
