#include "../include/Window.hpp"
#include <iostream>

#ifdef _WIN32
#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#if defined(HAVE_GLAD)
#include <glad/glad.h>
#endif
// Use GLAD to load OpenGL functions at runtime when available; otherwise GLStub is used.

namespace Titan {

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_CLOSE:
            PostQuitMessage(0);
            return 0;
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

bool Win32Window::Create(const std::string& title, uint32_t w, uint32_t h) {
    width = w;
    height = h;

    const char CLASS_NAME[] = "TitanEngineWindow";

    WNDCLASSA wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.lpszClassName = CLASS_NAME;
    wc.hInstance = GetModuleHandle(nullptr);
    wc.hCursor = LoadCursorA(nullptr, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

    if (!RegisterClassA(&wc)) {
        std::cerr << "Failed to register window class" << std::endl;
        return false;
    }

    hwnd = CreateWindowExA(
        0,                              // Optional window styles
        CLASS_NAME,                     // Window class
        title.c_str(),                  // Window text
        WS_OVERLAPPEDWINDOW,            // Window style

        // Size and position
        CW_USEDEFAULT, CW_USEDEFAULT, w, h,

        NULL,       // Parent window
        NULL,       // Menu
        GetModuleHandle(nullptr),
        NULL        // Pointer to pass to WM_CREATE
    );

    if (!hwnd) {
        std::cerr << "Failed to create window" << std::endl;
        return false;
    }

    if (!InitializeOpenGL()) {
        std::cerr << "Failed to initialize OpenGL" << std::endl;
        return false;
    }

    ShowWindow((HWND)hwnd, SW_SHOW);
    UpdateWindow((HWND)hwnd);

    isOpen = true;
    return true;
}

bool Win32Window::InitializeOpenGL() {
    hdc = (void*)GetDC((HWND)hwnd);
    if (!hdc) {
        std::cerr << "Failed to get device context" << std::endl;
        return false;
    }

    PIXELFORMATDESCRIPTOR pfd = {};
    pfd.nSize = sizeof(pfd);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 32;
    pfd.cDepthBits = 24;
    pfd.cStencilBits = 8;
    pfd.iLayerType = PFD_MAIN_PLANE;

    int pixelFormat = ChoosePixelFormat((HDC)hdc, &pfd);
    if (pixelFormat == 0) {
        std::cerr << "Failed to choose pixel format" << std::endl;
        return false;
    }

    if (!SetPixelFormat((HDC)hdc, pixelFormat, &pfd)) {
        std::cerr << "Failed to set pixel format" << std::endl;
        return false;
    }

    hglrc = (void*)wglCreateContext((HDC)hdc);
    if (!hglrc) {
        std::cerr << "Failed to create OpenGL context" << std::endl;
        return false;
    }

    if (!wglMakeCurrent((HDC)hdc, (HGLRC)hglrc)) {
        std::cerr << "Failed to make OpenGL context current" << std::endl;
        return false;
    }

    // Initialize GL loader if available
#if defined(HAVE_GLAD)
    if (!gladLoadGLLoader((GLADloadproc)wglGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return false;
    }
#endif

    return true;
}

void Win32Window::Destroy() {
    if (hglrc) {
            wglDeleteContext((HGLRC)hglrc);
            hglrc = nullptr;
        }

    if (hdc) {
        ReleaseDC((HWND)hwnd, (HDC)hdc);
        hdc = nullptr;
    }

    if (hwnd) {
        DestroyWindow((HWND)hwnd);
        hwnd = nullptr;
    }

    isOpen = false;
}

// `IsOpen()` defined inline in header; avoid duplicate definition here.

void Win32Window::Update() {
    MSG msg = {};
    while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
        if (msg.message == WM_QUIT) {
            isOpen = false;
            break;
        }
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

void Win32Window::SwapBuffers() {
    if (hdc) {
        ::SwapBuffers((HDC)hdc);
    }
}

void Win32Window::SetSize(uint32_t w, uint32_t h) {
    width = w;
    height = h;
    if (hwnd) {
        SetWindowPos((HWND)hwnd, nullptr, 0, 0, w, h, SWP_NOMOVE | SWP_NOZORDER);
    }
}

void Win32Window::SetTitle(const std::string& title) {
    if (hwnd) {
           SetWindowTextA((HWND)hwnd, title.c_str());
    }
}

void Win32Window::SetVSync(bool enabled) {
    vsync = enabled;
    if (hglrc) {
        // wglSwapIntervalEXT would be called here if available
        // for now this is a placeholder
    }
}

// `GetNativeHandle()` defined inline in header; avoid duplicate definition here.

} // namespace Titan

#endif // _WIN32
