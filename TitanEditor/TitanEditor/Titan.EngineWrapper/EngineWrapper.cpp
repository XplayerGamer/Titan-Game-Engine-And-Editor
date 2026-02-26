#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include "EngineWrapper.h"
#include <msclr/marshal_cppstd.h>

namespace TitanEngineWrapper {

    EngineWrapper::EngineWrapper()
        : m_engineHandle(nullptr), m_initialized(false), m_dllHandle(nullptr),
          m_createEngine(nullptr), m_destroyEngine(nullptr), m_initializeEngine(nullptr), m_shutdownEngine(nullptr),
          m_updateEngine(nullptr), m_renderFrame(nullptr), m_createEntity(nullptr), m_destroyEntity(nullptr),
          m_setEntityPosition(nullptr), m_getEntityPosition(nullptr), m_setCameraPosition(nullptr), m_setCameraRotation(nullptr),
          m_initializePhysics(nullptr), m_updatePhysics(nullptr), m_loadScript(nullptr), m_executeScript(nullptr)
    {
    }

    EngineWrapper::~EngineWrapper()
    {
        this->!EngineWrapper();
    }

    EngineWrapper::!EngineWrapper()
    {
        if (m_engineHandle && m_destroyEngine)
        {
            m_destroyEngine(m_engineHandle);
            m_engineHandle = nullptr;
        }
        UnloadEngineDLL();
    }

    EngineWrapper^ EngineWrapper::GetInstance()
    {
        if (s_instance == nullptr)
        {
            s_instance = gcnew EngineWrapper();
        }
        return s_instance;
    }

    bool EngineWrapper::LoadEngineDLL()
    {
        if (m_dllHandle != nullptr)
            return true;

        // Try to load TitanEngine.dll
        m_dllHandle = LoadLibraryA("TitanEngine.dll");
        if (m_dllHandle == nullptr)
        {
            DWORD error = GetLastError();
            throw gcnew System::Exception("Failed to load TitanEngine.dll. Error code: " + error.ToString());
        }

        // Get function pointers
        m_createEngine = (CreateEngineFunc)GetProcAddress(m_dllHandle, "CreateEngine");
        m_destroyEngine = (DestroyEngineFunc)GetProcAddress(m_dllHandle, "DestroyEngine");
        m_initializeEngine = (InitializeEngineFunc)GetProcAddress(m_dllHandle, "InitializeEngine");
        m_shutdownEngine = (ShutdownEngineFunc)GetProcAddress(m_dllHandle, "ShutdownEngine");
        m_updateEngine = (UpdateEngineFunc)GetProcAddress(m_dllHandle, "UpdateEngine");
        m_renderFrame = (RenderFrameFunc)GetProcAddress(m_dllHandle, "RenderFrame");
        m_createEntity = (CreateEntityFunc)GetProcAddress(m_dllHandle, "CreateEntity");
        m_destroyEntity = (DestroyEntityFunc)GetProcAddress(m_dllHandle, "DestroyEntity");
        m_setEntityPosition = (SetEntityPositionFunc)GetProcAddress(m_dllHandle, "SetEntityPosition");
        m_getEntityPosition = (GetEntityPositionFunc)GetProcAddress(m_dllHandle, "GetEntityPosition");
        m_setCameraPosition = (SetCameraPositionFunc)GetProcAddress(m_dllHandle, "SetCameraPosition");
        m_setCameraRotation = (SetCameraRotationFunc)GetProcAddress(m_dllHandle, "SetCameraRotation");
        m_initializePhysics = (InitializePhysicsFunc)GetProcAddress(m_dllHandle, "InitializePhysics");
        m_updatePhysics = (UpdatePhysicsFunc)GetProcAddress(m_dllHandle, "UpdatePhysics");
        m_loadScript = (LoadScriptFunc)GetProcAddress(m_dllHandle, "LoadScript");
        m_executeScript = (ExecuteScriptFunc)GetProcAddress(m_dllHandle, "ExecuteScript");

        if (!m_createEngine || !m_destroyEngine || !m_initializeEngine || !m_shutdownEngine)
        {
            UnloadEngineDLL();
            throw gcnew System::Exception("Failed to find required functions in TitanEngine.dll");
        }

        return true;
    }

    void EngineWrapper::UnloadEngineDLL()
    {
        if (m_dllHandle != nullptr)
        {
            FreeLibrary(m_dllHandle);
            m_dllHandle = nullptr;
            m_createEngine = nullptr;
            m_destroyEngine = nullptr;
            m_initializeEngine = nullptr;
            m_shutdownEngine = nullptr;
            m_updateEngine = nullptr;
            m_renderFrame = nullptr;
            m_createEntity = nullptr;
            m_destroyEntity = nullptr;
            m_setEntityPosition = nullptr;
            m_getEntityPosition = nullptr;
            m_setCameraPosition = nullptr;
            m_setCameraRotation = nullptr;
            m_initializePhysics = nullptr;
            m_updatePhysics = nullptr;
            m_loadScript = nullptr;
            m_executeScript = nullptr;
        }
    }

    void EngineWrapper::Initialize()
    {
        EngineWrapper^ instance = GetInstance();
        if (!instance->m_initialized)
        {
            try
            {
                // Load the DLL first
                instance->LoadEngineDLL();

                // Create engine instance
                instance->m_engineHandle = instance->m_createEngine();
                if (!instance->m_engineHandle)
                {
                    throw gcnew System::Exception("Failed to create engine instance");
                }

                // Initialize engine
                if (instance->m_initializeEngine(instance->m_engineHandle, "Titan Editor", 1280, 720, 60, true, false))
                {
                    instance->m_initialized = true;
                }
                else
                {
                    throw gcnew System::Exception("Engine initialization failed");
                }
            }
            catch (System::Exception^ ex)
            {
                throw gcnew System::Exception("Failed to initialize Titan Engine: " + ex->Message);
            }
            catch (...)
            {
                throw gcnew System::Exception("Unknown error occurred while initializing Titan Engine");
            }
        }
    }

    void EngineWrapper::Shutdown()
    {
        EngineWrapper^ instance = GetInstance();
        if (instance->m_initialized && instance->m_engineHandle && instance->m_shutdownEngine)
        {
            instance->m_shutdownEngine(instance->m_engineHandle);
            instance->m_initialized = false;
        }
    }

    void EngineWrapper::Update(float deltaTime)
    {
        EngineWrapper^ instance = GetInstance();
        if (instance->m_initialized && instance->m_engineHandle && instance->m_updateEngine)
        {
            instance->m_updateEngine(instance->m_engineHandle, deltaTime);
        }
    }

    void EngineWrapper::InitializePhysics()
    {
        EngineWrapper^ instance = GetInstance();
        if (instance->m_initialized && instance->m_engineHandle && instance->m_initializePhysics)
        {
            instance->m_initializePhysics(instance->m_engineHandle);
        }
    }

    void EngineWrapper::UpdatePhysics(float deltaTime)
    {
        EngineWrapper^ instance = GetInstance();
        if (instance->m_initialized && instance->m_engineHandle && instance->m_updatePhysics)
        {
            instance->m_updatePhysics(instance->m_engineHandle, deltaTime);
        }
    }

    void EngineWrapper::InitializeRenderer(IntPtr windowHandle, int width, int height)
    {
        // Renderer is initialized as part of engine initialization
    }

    void EngineWrapper::RenderFrame()
    {
        EngineWrapper^ instance = GetInstance();
        if (instance->m_initialized && instance->m_engineHandle && instance->m_renderFrame)
        {
            instance->m_renderFrame(instance->m_engineHandle);
        }
    }

    void EngineWrapper::SetCameraPosition(float x, float y, float z)
    {
        EngineWrapper^ instance = GetInstance();
        if (instance->m_initialized && instance->m_engineHandle && instance->m_setCameraPosition)
        {
            instance->m_setCameraPosition(instance->m_engineHandle, x, y, z);
        }
    }

    void EngineWrapper::SetCameraRotation(float yaw, float pitch)
    {
        EngineWrapper^ instance = GetInstance();
        if (instance->m_initialized && instance->m_engineHandle && instance->m_setCameraRotation)
        {
            instance->m_setCameraRotation(instance->m_engineHandle, yaw, pitch);
        }
    }

    int EngineWrapper::CreateEntity()
    {
        EngineWrapper^ instance = GetInstance();
        if (instance->m_initialized && instance->m_engineHandle && instance->m_createEntity)
        {
            return instance->m_createEntity(instance->m_engineHandle);
        }
        return -1;
    }

    void EngineWrapper::DestroyEntity(int entityId)
    {
        EngineWrapper^ instance = GetInstance();
        if (instance->m_initialized && instance->m_engineHandle && instance->m_destroyEntity)
        {
            instance->m_destroyEntity(instance->m_engineHandle, entityId);
        }
    }

    void EngineWrapper::SetEntityPosition(int entityId, float x, float y, float z)
    {
        EngineWrapper^ instance = GetInstance();
        if (instance->m_initialized && instance->m_engineHandle && instance->m_setEntityPosition)
        {
            instance->m_setEntityPosition(instance->m_engineHandle, entityId, x, y, z);
        }
    }

    void EngineWrapper::GetEntityPosition(int entityId, [System::Runtime::InteropServices::Out] float% x, [System::Runtime::InteropServices::Out] float% y, [System::Runtime::InteropServices::Out] float% z)
    {
        EngineWrapper^ instance = GetInstance();
        if (instance->m_initialized && instance->m_engineHandle && instance->m_getEntityPosition)
        {
            float px, py, pz;
            instance->m_getEntityPosition(instance->m_engineHandle, entityId, &px, &py, &pz);
            x = px;
            y = py;
            z = pz;
        }
        else
        {
            x = 0.0f;
            y = 0.0f;
            z = 0.0f;
        }
    }

    bool EngineWrapper::LoadScript(String^ scriptPath)
    {
        EngineWrapper^ instance = GetInstance();
        if (instance->m_initialized && instance->m_engineHandle && instance->m_loadScript)
        {
            msclr::interop::marshal_context context;
            const char* nativePath = context.marshal_as<const char*>(scriptPath);
            return instance->m_loadScript(instance->m_engineHandle, nativePath);
        }
        return false;
    }

    bool EngineWrapper::ExecuteScript(String^ scriptContent)
    {
        EngineWrapper^ instance = GetInstance();
        if (instance->m_initialized && instance->m_engineHandle && instance->m_executeScript)
        {
            msclr::interop::marshal_context context;
            const char* nativeContent = context.marshal_as<const char*>(scriptContent);
            return instance->m_executeScript(instance->m_engineHandle, nativeContent);
        }
        return false;
    }
}