cmake_minimum_required(VERSION 3.16)
project(TitanEngine VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ============================================================================
# Dependencies
# ============================================================================

# Add GLM (header-only)
set(GLM_INCLUDE_DIRS "${CMAKE_SOURCE_DIR}/../Linking/include")

# Lua
# Use Lua from E drive installation (runtime loading, not compile-time linking)
set(LUA_INCLUDE_DIR "E:/lua-5.4.6/install/include")
# Remove LUA_LIBRARIES - we'll load Lua dynamically at runtime

# Vulkan detection: disabled for now to use stub implementation
# find_package(Vulkan REQUIRED)
# if (Vulkan_FOUND)
#     message(STATUS "Using system Vulkan SDK: ${Vulkan_INCLUDE_DIRS}")
#     add_definitions(-DVULKAN_SDK=1)
# else()
    message(STATUS "Using Vulkan stub implementation (no Vulkan SDK)")
# endif()

# Bullet Physics
set(BULLET_INCLUDE_DIRS "E:/bullet3-3.25/install/include")
set(BULLET_LIBRARIES 
    "E:/bullet3-3.25/install/lib/LinearMath.lib"
    "E:/bullet3-3.25/install/lib/Bullet3Common.lib"
    "E:/bullet3-3.25/install/lib/Bullet3Collision.lib"
    "E:/bullet3-3.25/install/lib/Bullet3Dynamics.lib"
)
add_definitions(-DHAVE_BULLET=1)
add_definitions(-DGLM_ENABLE_EXPERIMENTAL)

# ============================================================================
# Engine Library
# ============================================================================

set(TITAN_HEADERS
    include/Core.hpp
    include/Engine.hpp
    include/Renderer.hpp
    include/Input.hpp
    include/Physics.hpp
    include/Scripting.hpp
    include/Audio.hpp
    include/Window.hpp
    include/Networking.hpp
    include/Weapons.hpp
    include/Gamemodes.hpp
    include/Performance.hpp
    include/Effects.hpp
    include/VulkanRenderer.hpp
    include/HammerEditor.hpp
    include/TitanEditor.hpp
    include/CoreMath.hpp
    include/TitanUtils.hpp
)

set(TITAN_SOURCES
    src/Core.cpp
    src/Engine.cpp
    src/Renderer.cpp
    src/Input.cpp
    src/Physics.cpp
    src/Scripting.cpp
    src/Audio.cpp
    src/Window.cpp
    src/Weapons.cpp
    src/Gamemodes.cpp
    src/Performance.cpp
    src/Effects.cpp
    src/VulkanRenderer.cpp
    src/HammerEditor.cpp
    src/TitanEditor.cpp
    src/CoreMath.cpp
    src/TitanUtils.cpp
    src/LuaStub.cpp
)

add_library(TitanEngine SHARED ${TITAN_SOURCES} ${TITAN_HEADERS})

target_compile_definitions(TitanEngine PRIVATE TITANENGINE_EXPORTS)

# ============================================================================
# GLAD (OpenGL loader) - fetched at configure time if Git is available
# ============================================================================
find_program(GIT_EXECUTABLE git)
if(GIT_EXECUTABLE)
        include(FetchContent)
        FetchContent_Declare(
            glad
            GIT_REPOSITORY https://github.com/Dav1dde/glad.git
            GIT_TAG v0.1.36
        )
        FetchContent_MakeAvailable(glad)
        target_link_libraries(TitanEngine PUBLIC glad)
        target_compile_definitions(TitanEngine PUBLIC HAVE_GLAD=1)
else()
        message(WARNING "Git not found; skipping GLAD fetch. Falling back to GL stub (no runtime GL).")
endif()

target_include_directories(TitanEngine PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${GLM_INCLUDE_DIRS}
    ${LUA_INCLUDE_DIR}
    ${BULLET_INCLUDE_DIRS}
)

target_link_libraries(TitanEngine PUBLIC
    ${BULLET_LIBRARIES}
    opengl32.lib
 )

if (Vulkan_FOUND)
    target_link_libraries(TitanEngine PUBLIC ${Vulkan_LIBRARIES})
endif()

if(MSVC)
    target_compile_options(TitanEngine PRIVATE /W4)
else()
    target_compile_options(TitanEngine PRIVATE -Wall -Wextra)
endif()

# ============================================================================
# Example Game
# ============================================================================

add_executable(TitanGame
    example/main.cpp
    example/ExampleGame.hpp
    example/ExampleGame.cpp
)

target_include_directories(TitanGame PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${GLM_INCLUDE_DIRS}
)

target_link_libraries(TitanGame PRIVATE
    TitanEngine
)

add_executable(NetworkTest
    example/NetworkTest.cpp
)

target_link_libraries(NetworkTest PRIVATE
    TitanEngine
)

add_executable(HammerEditorApp
    example/EditorMain.cpp
)

target_link_libraries(HammerEditorApp PRIVATE
    TitanEngine
)

add_executable(TitanEditorApp
    example/TitanEditorMain.cpp
)

target_link_libraries(TitanEditorApp PRIVATE
    TitanEngine
)

# ============================================================================
# Test Suite
# ============================================================================

add_executable(TitanTests
    src/Tests.cpp
)

target_include_directories(TitanTests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(TitanTests PRIVATE
    TitanEngine
)

if(WIN32)
    target_link_libraries(TitanTests PRIVATE
        user32
        gdi32
        opengl32
    )
endif()

# ============================================================================
# Installation
# ============================================================================

install(TARGETS TitanEngine TitanGame
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
)

install(DIRECTORY include/ DESTINATION include/Titan)
