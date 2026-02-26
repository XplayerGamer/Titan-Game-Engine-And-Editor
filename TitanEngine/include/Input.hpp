#pragma once

#include "Core.hpp"
#include <unordered_set>

namespace Titan {

// ============================================================================
// Input Codes
// ============================================================================

enum class KeyCode {
    // Letters
    A = 65, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
    
    // Numbers
    Num0 = 48, Num1, Num2, Num3, Num4, Num5, Num6, Num7, Num8, Num9,
    
    // Function keys
    F1 = 112, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,
    
    // Special keys
    Escape = 27,
    Tab = 9,
    Backspace = 8,
    Enter = 13,
    Space = 32,
    LeftShift = 160,
    RightShift = 161,
    LeftCtrl = 162,
    RightCtrl = 163,
    LeftAlt = 164,
    RightAlt = 165,
    
    // Arrow keys
    Left = 37,
    Up = 38,
    Right = 39,
    Down = 40,
};

enum class MouseButton {
    Left = 0,
    Right = 1,
    Middle = 2,
};

// ============================================================================
// Input Events
// ============================================================================

struct KeyPressedEvent : public Event {
    KeyCode key;
    bool repeated{false};

    explicit KeyPressedEvent(KeyCode k) : Event(1001), key(k) {}
};

struct KeyReleasedEvent : public Event {
    KeyCode key;

    explicit KeyReleasedEvent(KeyCode k) : Event(1002), key(k) {}
};

struct MouseMovedEvent : public Event {
    float x{0.0f};
    float y{0.0f};
    float deltaX{0.0f};
    float deltaY{0.0f};

    MouseMovedEvent() : Event(1003) {}
};

struct MouseButtonPressedEvent : public Event {
    MouseButton button;

    explicit MouseButtonPressedEvent(MouseButton btn) : Event(1004), button(btn) {}
};

struct MouseButtonReleasedEvent : public Event {
    MouseButton button;

    explicit MouseButtonReleasedEvent(MouseButton btn) : Event(1005), button(btn) {}
};

struct MouseScrollEvent : public Event {
    float scrollDelta{0.0f};

    explicit MouseScrollEvent(float delta) : Event(1006), scrollDelta(delta) {}
};

// ============================================================================
// Input System Interface
// ============================================================================

class InputSystem : public ISystem {
public:
    virtual ~InputSystem() = default;

    virtual void Initialize() override = 0;
    virtual void Update(float deltaTime) override = 0;
    virtual void Shutdown() override = 0;

    virtual bool IsKeyPressed(KeyCode key) const = 0;
    virtual bool IsKeyReleased(KeyCode key) const = 0;
    virtual bool IsMouseButtonPressed(MouseButton button) const = 0;
    virtual bool IsMouseButtonReleased(MouseButton button) const = 0;

    virtual void GetMousePosition(float& x, float& y) const = 0;
    virtual void GetMouseDelta(float& deltaX, float& deltaY) const = 0;

    virtual void SetInputLocked(bool locked) = 0;
    virtual bool IsInputLocked() const = 0;
};

// ============================================================================
// Simple Input System Implementation
// ============================================================================

class SimpleInputSystem : public InputSystem {
private:
    std::unordered_set<int> pressedKeys;
    std::unordered_set<int> releasedKeys;
    std::unordered_set<int> pressedMouseButtons;
    std::unordered_set<int> releasedMouseButtons;
    
    float mouseX{0.0f};
    float mouseY{0.0f};
    float mouseDeltaX{0.0f};
    float mouseDeltaY{0.0f};
    float scrollDelta{0.0f};
    
    bool inputLocked{false};

public:
    void Initialize() override;
    void Update(float deltaTime) override;
    void Shutdown() override;

    bool IsKeyPressed(KeyCode key) const override;
    bool IsKeyReleased(KeyCode key) const override;
    bool IsMouseButtonPressed(MouseButton button) const override;
    bool IsMouseButtonReleased(MouseButton button) const override;

    void GetMousePosition(float& x, float& y) const override;
    void GetMouseDelta(float& deltaX, float& deltaY) const override;

    void SetInputLocked(bool locked) override { inputLocked = locked; }
    bool IsInputLocked() const override { return inputLocked; }

    // Internal methods for platform-specific input handling
    void OnKeyPressed(KeyCode key);
    void OnKeyReleased(KeyCode key);
    void OnMouseMoved(float x, float y);
    void OnMouseButtonPressed(MouseButton button);
    void OnMouseButtonReleased(MouseButton button);
    void OnMouseScroll(float delta);
};

} // namespace Titan
