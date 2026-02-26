#pragma once

#include <string>
#include <memory>
#include <vector>
#include <glm/glm.hpp>

namespace Titan {

// ============================================================================
// Window Interface
// ============================================================================

class Window {
public:
    virtual ~Window() = default;

    virtual bool Create(const std::string& title, uint32_t width, uint32_t height) = 0;
    virtual void Destroy() = 0;
    virtual bool IsOpen() const = 0;
    virtual void Close() = 0;
    virtual void Update() = 0;
    virtual void SwapBuffers() = 0;

    virtual uint32_t GetWidth() const = 0;
    virtual uint32_t GetHeight() const = 0;
    virtual void SetSize(uint32_t width, uint32_t height) = 0;

    virtual void SetTitle(const std::string& title) = 0;
    virtual void SetVSync(bool enabled) = 0;

    virtual void* GetNativeHandle() = 0;
};

// ============================================================================
// Platform-specific Window Implementations
// ============================================================================

#ifdef _WIN32

class Win32Window : public Window {
private:
    void* hwnd{nullptr};
    void* hdc{nullptr};
    void* hglrc{nullptr};
    uint32_t width{0};
    uint32_t height{0};
    bool isOpen{false};
    bool vsync{false};

public:
    bool Create(const std::string& title, uint32_t w, uint32_t h) override;
    void Destroy() override;
    bool IsOpen() const override { return isOpen; }
    void Close() override { isOpen = false; }
    void Update() override;
    void SwapBuffers() override;

    uint32_t GetWidth() const override { return width; }
    uint32_t GetHeight() const override { return height; }
    void SetSize(uint32_t w, uint32_t h) override;

    void SetTitle(const std::string& title) override;
    void SetVSync(bool enabled) override;

    void* GetNativeHandle() override { return hwnd; }

private:
    bool InitializeOpenGL();
};

#endif

} // namespace Titan
