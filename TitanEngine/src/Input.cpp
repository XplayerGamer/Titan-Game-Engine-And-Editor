#include "../include/Input.hpp"
#include <iostream>

namespace Titan {

// ============================================================================
// SimpleInputSystem Implementation
// ============================================================================

void SimpleInputSystem::Initialize() {
    std::cout << "Input system initialized" << std::endl;
}

void SimpleInputSystem::Update(float deltaTime) {
    // Clear released states from previous frame
    releasedKeys.clear();
    releasedMouseButtons.clear();
    mouseDeltaX = 0.0f;
    mouseDeltaY = 0.0f;
    scrollDelta = 0.0f;

    // In a real implementation, this would poll the OS for input events
    // and update the input state accordingly
}

void SimpleInputSystem::Shutdown() {
    std::cout << "Input system shutdown" << std::endl;
    pressedKeys.clear();
    releasedKeys.clear();
    pressedMouseButtons.clear();
    releasedMouseButtons.clear();
}

bool SimpleInputSystem::IsKeyPressed(KeyCode key) const {
    return pressedKeys.find(static_cast<int>(key)) != pressedKeys.end();
}

bool SimpleInputSystem::IsKeyReleased(KeyCode key) const {
    return releasedKeys.find(static_cast<int>(key)) != releasedKeys.end();
}

bool SimpleInputSystem::IsMouseButtonPressed(MouseButton button) const {
    return pressedMouseButtons.find(static_cast<int>(button)) != pressedMouseButtons.end();
}

bool SimpleInputSystem::IsMouseButtonReleased(MouseButton button) const {
    return releasedMouseButtons.find(static_cast<int>(button)) != releasedMouseButtons.end();
}

void SimpleInputSystem::GetMousePosition(float& x, float& y) const {
    x = mouseX;
    y = mouseY;
}

void SimpleInputSystem::GetMouseDelta(float& deltaX, float& deltaY) const {
    deltaX = mouseDeltaX;
    deltaY = mouseDeltaY;
}

void SimpleInputSystem::OnKeyPressed(KeyCode key) {
    pressedKeys.insert(static_cast<int>(key));
}

void SimpleInputSystem::OnKeyReleased(KeyCode key) {
    pressedKeys.erase(static_cast<int>(key));
    releasedKeys.insert(static_cast<int>(key));
}

void SimpleInputSystem::OnMouseMoved(float x, float y) {
    mouseDeltaX = x - mouseX;
    mouseDeltaY = y - mouseY;
    mouseX = x;
    mouseY = y;
}

void SimpleInputSystem::OnMouseButtonPressed(MouseButton button) {
    pressedMouseButtons.insert(static_cast<int>(button));
}

void SimpleInputSystem::OnMouseButtonReleased(MouseButton button) {
    pressedMouseButtons.erase(static_cast<int>(button));
    releasedMouseButtons.insert(static_cast<int>(button));
}

void SimpleInputSystem::OnMouseScroll(float delta) {
    scrollDelta = delta;
}

} // namespace Titan
