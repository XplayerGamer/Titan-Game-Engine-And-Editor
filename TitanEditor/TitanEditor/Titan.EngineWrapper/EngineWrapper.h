#pragma once

#include <Windows.h>
#include <string>

using namespace System;

namespace TitanEngineWrapper {

    // Function pointer types for runtime DLL loading
    typedef void* (*CreateEngineFunc)();
    typedef void (*DestroyEngineFunc)(void* engine);
    typedef bool (*InitializeEngineFunc)(void* engine, const char* appName, int width, int height, int targetFPS, bool vsync, bool headless);
    typedef void (*ShutdownEngineFunc)(void* engine);
    typedef void (*UpdateEngineFunc)(void* engine, float deltaTime);
    typedef void (*RenderFrameFunc)(void* engine);
    typedef int (*CreateEntityFunc)(void* engine);
    typedef void (*DestroyEntityFunc)(void* engine, int entityId);
    typedef void (*SetEntityPositionFunc)(void* engine, int entityId, float x, float y, float z);
    typedef void (*GetEntityPositionFunc)(void* engine, int entityId, float* x, float* y, float* z);
    typedef void (*SetCameraPositionFunc)(void* engine, float x, float y, float z);
    typedef void (*SetCameraRotationFunc)(void* engine, float yaw, float pitch);
    typedef void (*InitializePhysicsFunc)(void* engine);
    typedef void (*UpdatePhysicsFunc)(void* engine, float deltaTime);
    typedef bool (*LoadScriptFunc)(void* engine, const char* scriptPath);
    typedef bool (*ExecuteScriptFunc)(void* engine, const char* scriptContent);

    public ref class EngineWrapper
    {
    private:
        static EngineWrapper^ s_instance = nullptr;
        void* m_engineHandle;
        bool m_initialized;
        HMODULE m_dllHandle;

        // Function pointers
        CreateEngineFunc m_createEngine;
        DestroyEngineFunc m_destroyEngine;
        InitializeEngineFunc m_initializeEngine;
        ShutdownEngineFunc m_shutdownEngine;
        UpdateEngineFunc m_updateEngine;
        RenderFrameFunc m_renderFrame;
        CreateEntityFunc m_createEntity;
        DestroyEntityFunc m_destroyEntity;
        SetEntityPositionFunc m_setEntityPosition;
        GetEntityPositionFunc m_getEntityPosition;
        SetCameraPositionFunc m_setCameraPosition;
        SetCameraRotationFunc m_setCameraRotation;
        InitializePhysicsFunc m_initializePhysics;
        UpdatePhysicsFunc m_updatePhysics;
        LoadScriptFunc m_loadScript;
        ExecuteScriptFunc m_executeScript;

    public:
        EngineWrapper();
        ~EngineWrapper();
        !EngineWrapper();

        static void Initialize();
        static void Shutdown();
        void Update(float deltaTime);

        // Physics
        void InitializePhysics();
        void UpdatePhysics(float deltaTime);

        // Rendering
        void InitializeRenderer(IntPtr windowHandle, int width, int height);
        void RenderFrame();
        void SetCameraPosition(float x, float y, float z);
        void SetCameraRotation(float yaw, float pitch);

        // Entity management
        int CreateEntity();
        void DestroyEntity(int entityId);
        void SetEntityPosition(int entityId, float x, float y, float z);
        void GetEntityPosition(int entityId, [System::Runtime::InteropServices::Out] float% x, [System::Runtime::InteropServices::Out] float% y, [System::Runtime::InteropServices::Out] float% z);

        // Scripting
        bool LoadScript(String^ scriptPath);
        bool ExecuteScript(String^ scriptContent);

    private:
        static EngineWrapper^ GetInstance();
        bool LoadEngineDLL();
        void UnloadEngineDLL();
    };

}