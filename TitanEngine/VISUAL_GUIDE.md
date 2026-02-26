# Titan Engine - Visual Guide

## System Architecture Diagram

```
┌─────────────────────────────────────────────────────────────────────┐
│                        TITAN ENGINE                                 │
└─────────────────────────────────────────────────────────────────────┘
                              │
                    ┌─────────┴─────────┐
                    ▼                   ▼
         ┌──────────────────┐   ┌──────────────────┐
         │   Engine Loop    │   │   Entity Manager │
         ├──────────────────┤   ├──────────────────┤
         │ - Delta Time     │   │ - CreateEntity   │
         │ - FPS Limiting   │   │ - DestroyEntity  │
         │ - System Updates │   │ - Component Mgmt │
         │ - Frame Render   │   │ - Query API      │
         └──────────────────┘   └──────────────────┘
                    │
        ┌───────────┼───────────┐
        │           │           │
        ▼           ▼           ▼
    ┌───────┐  ┌──────────┐  ┌─────────┐
    │ Input │  │ Physics  │  │Scripting│
    └───────┘  └──────────┘  └─────────┘
        │           │           │
        └───────────┼───────────┘
                    ▼
            ┌──────────────────┐
            │   Event Bus      │
            │  (Communication) │
            └──────────────────┘
                    │
        ┌───────────┼───────────┐
        │           │           │
        ▼           ▼           ▼
    ┌───────┐  ┌──────────┐  ┌─────────┐
    │ Audio │  │Renderer  │  │ Window  │
    └───────┘  └──────────┘  └─────────┘
```

## Entity Component Relationships

```
                    Entity (ID: 1)
                         │
        ┌────────────────┼────────────────┐
        ▼                ▼                ▼
    ┌──────────┐   ┌──────────────┐  ┌──────────────┐
    │Transform │   │  RigidBody   │  │ Renderable   │
    ├──────────┤   ├──────────────┤  ├──────────────┤
    │pos: vec3 │   │vel: vec3     │  │mesh: string  │
    │rot: vec3 │   │acc: vec3     │  │mat: string   │
    │sca: vec3 │   │mass: float   │  │visible: bool │
    └──────────┘   │gravity: bool │  └──────────────┘
                   │kinematic:boo │
                   └──────────────┘
```

## Data Flow Per Frame

```
START FRAME
    │
    ├─ Calculate Delta Time
    │   └─ Time elapsed since last frame
    │
    ├─ Update Systems (in order):
    │   ├─ InputSystem
    │   │  └─ Poll keyboard/mouse
    │   │
    │   ├─ PhysicsSystem
    │   │  ├─ Iterate entities with RigidBody
    │   │  ├─ Apply gravity and forces
    │   │  └─ Update positions
    │   │
    │   ├─ ScriptingSystem
    │   │  ├─ Call Lua OnUpdate(deltaTime)
    │   │  └─ Execute script logic
    │   │
    │   ├─ AudioSystem
    │   │  ├─ Update playback
    │   │  └─ Update 3D positions
    │   │
    │   └─ Renderer
    │      └─ Prepare frame data
    │
    ├─ Render Frame
    │   ├─ BeginFrame (clear buffers)
    │   ├─ Submit all entities
    │   ├─ EndFrame (finalize)
    │   └─ Present (swap buffers)
    │
    └─ NEXT FRAME
```

## Component Hierarchy

```
┌────────────────────────────────────────────┐
│         Component (Abstract)               │
│    ┌─ GetComponentID(): ComponentID        │
└────────────────────────────────────────────┘
        ▲         ▲         ▲           ▲
        │         │         │           │
        │         │         │           │
    ┌───────┐ ┌────────┐ ┌──────────┐ ┌────────────┐
    │Transform│AudioSource│Renderable│ RigidBody  │
    ├───────┤ ├────────┤ ├──────────┤ ├────────────┤
    │pos    │ │path    │ │mesh      │ │velocity    │
    │rot    │ │volume  │ │material  │ │mass        │
    │scale  │ │playing │ │visible   │ │gravity     │
    └───────┘ └────────┘ └──────────┘ └────────────┘
```

## System Lifecycle

```
                    Engine Initialize()
                            │
            ┌───────────────┬┴┬───────────────┐
            ▼               ▼               ▼
    ┌──────────────┐ ┌──────────────┐ ┌──────────────┐
    │ Renderer     │ │ InputSystem  │ │ PhysicsSystem│
    │ Initialize() │ │Initialize()  │ │Initialize() │
    └──────────────┘ └──────────────┘ └──────────────┘
            │               │               │
            └───────────────┼───────────────┘
                            │
                    ┌───────▼────────┐
                    │  Main Loop     │
                    │ Update()×60fps │
                    └────────────────┘
                            │
            ┌───────────────┬┴┬───────────────┐
            ▼               ▼               ▼
    ┌──────────────┐ ┌──────────────┐ ┌──────────────┐
    │ Renderer     │ │ InputSystem  │ │ PhysicsSystem│
    │ Update()     │ │Update()      │ │Update()      │
    └──────────────┘ └──────────────┘ └──────────────┘
            │               │               │
            └───────────────┼───────────────┘
                            │
                    Engine Shutdown()
```

## Event System Flow

```
Publisher                 Event Bus              Subscribers
    │                         │                      │
    ├─ CreateEvent()          │                      │
    │     │                   │                      │
    │     └─ Publish(event)───┤                      │
    │                         ├─ Lookup subscribers──┤
    │                         │                      ├─ OnEvent()
    │                         │                      │
    │                         │                      ├─ OnEvent()
    │                         │                      │
    │                         └─ OnEvent()           │
    │                              │                 │
    └─────────────────────────────►│◄────────────────┘
```

## Rendering Pipeline

```
┌───────────────────────────────────┐
│  Per Entity:                      │
│  1. Get Transform                 │
│  2. Get Renderable                │
│  3. Get Model Matrix              │
│  4. Get Mesh & Material           │
└───────────────────────────────────┘
              │
              ▼
┌───────────────────────────────────┐
│  Renderer.SubmitMesh()            │
│  - Store mesh data                │
│  - Store transform                │
│  - Store material                 │
└───────────────────────────────────┘
              │
              ▼
┌───────────────────────────────────┐
│  Renderer.BeginFrame()            │
│  - Clear color buffer             │
│  - Clear depth buffer             │
└───────────────────────────────────┘
              │
              ▼
┌───────────────────────────────────┐
│  Render Loop (per mesh):          │
│  - Bind VAO                       │
│  - Set uniforms                   │
│  - DrawElements()                 │
└───────────────────────────────────┘
              │
              ▼
┌───────────────────────────────────┐
│  Renderer.EndFrame()              │
│  - Finalize rendering             │
└───────────────────────────────────┘
              │
              ▼
┌───────────────────────────────────┐
│  Window.SwapBuffers()             │
│  - Display frame                  │
└───────────────────────────────────┘
```

## Physics Update Loop

```
For each Entity with RigidBody:
    │
    ├─ Check if kinematic
    │   ├─ YES: Skip physics
    │   └─ NO: Continue
    │
    ├─ Apply gravity
    │   └─ acceleration += gravity
    │
    ├─ Apply forces
    │   └─ velocity += acceleration * dt
    │
    ├─ Apply drag
    │   └─ velocity *= 0.99f
    │
    ├─ Update position
    │   └─ position += velocity * dt
    │
    └─ Reset forces for next frame
        └─ acceleration = 0
```

## Lua Mod Lifecycle

```
┌─────────────────────┐
│  scripting.LoadMod()│
└──────────┬──────────┘
           │
           ▼
    ┌─────────────┐
    │  Execute    │
    │  Script     │
    └──────┬──────┘
           │
           ▼
    ┌──────────────────────┐
    │ Call OnModInit()     │
    │ (Initialization)     │
    └──────┬───────────────┘
           │
           ▼
    ┌──────────────────────┐
    │ Main Loop            │
    │ Call OnUpdate(dt)    │
    │ each frame           │
    └──────┬───────────────┘
           │
           ▼
    ┌──────────────────────┐
    │ scripting.UnloadMod()│
    └──────┬───────────────┘
           │
           ▼
    ┌──────────────────────┐
    │ Call OnModCleanup()  │
    │ (Cleanup)            │
    └──────────────────────┘
```

## Memory Layout

```
Stack:
┌────────────────────────────────┐
│ Local Variables                │
│ Function Parameters            │
│ Return Addresses               │
└────────────────────────────────┘

Heap:
┌────────────────────────────────┐
│ Engine Instance                │
│ ├─ EntityManager              │
│ │  └─ Entities (vector)       │
│ │     ├─ Components (map)     │
│ │     ├─ Components (map)     │
│ │     └─ ...                  │
│ │                              │
│ ├─ Systems                     │
│ │  ├─ Renderer                │
│ │  ├─ PhysicsSystem           │
│ │  └─ ...                     │
│ │                              │
│ └─ EventBus                    │
│    └─ Subscribers (map)        │
└────────────────────────────────┘
```

## Component Access Pattern

```
Get Entity
    │
    ▼
Entity::GetComponent<T>()
    │
    ├─ Compute T::StaticID()
    │
    ├─ Find ID in components map
    │
    ├─ Found?
    │   ├─ YES: Return as std::shared_ptr<T>
    │   └─ NO: Return nullptr
    │
    ▼
Use component data
```

## Input Event Queue

```
┌─ Physical Input ─┐
│  Keyboard press  │
│  Mouse movement  │
│  Button click    │
└─────────┬────────┘
          │
          ▼
    ┌──────────────┐
    │ OS captures  │
    │ input event  │
    └──────┬───────┘
           │
           ▼
    ┌──────────────────────┐
    │ InputSystem stores   │
    │ pressed/released     │
    │ state                │
    └──────┬───────────────┘
           │
           ▼
    ┌──────────────────────┐
    │ Game queries state   │
    │ IsKeyPressed()       │
    └──────────────────────┘
```

## Transform Coordinate System

```
        +Y (Up)
         │
         │  +Z (Forward)
         │ /
         │/
    ─────●───── +X (Right)
        /│
       / │
      /  │
     
Transform::GetForward()  = +Z direction
Transform::GetRight()    = +X direction  
Transform::GetUp()       = +Y direction
```

## System Priority Order

```
Frame Update Order (Critical):

1. InputSystem          → Capture user input
2. PhysicsSystem        → Simulate world
3. ScriptingSystem      → Execute game logic
4. AudioSystem          → Update audio playback
5. Renderer             → Render frame

This order ensures:
- Input is captured before logic processes it
- Physics updates before rendering
- Scripts can modify entity state
- Audio position matches visual
- Everything renders at frame end
```

---

These visual guides help understand the engine's architecture and data flow at a glance!
