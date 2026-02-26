#include "../include/TestFramework.hpp"
#include "../include/Core.hpp"
#include "../include/Renderer.hpp"
#include "../include/TitanEditor.hpp"
#include "../include/Networking.hpp"
#include <iostream>

using namespace Titan;
using namespace Titan::Test;

// ============================================================================
// Core System Tests
// ============================================================================

REGISTER_TEST(EntityManager_CreateEntity) {
    EntityManager em;
    auto id = em.CreateEntity("TestEntity");
    ASSERT(id != 0);
    auto entity = em.GetEntity(id);
    ASSERT_NOT_NULL(entity.get());
    ASSERT_STR_EQ(entity->GetName(), "TestEntity");
}

REGISTER_TEST(EntityManager_DestroyEntity) {
    EntityManager em;
    auto id = em.CreateEntity("ToDestroy");
    em.DestroyEntity(id);
    auto entity = em.GetEntity(id);
    ASSERT_NULL(entity.get());
}

REGISTER_TEST(Entity_AddComponent) {
    Entity e(1, "TestEntity");
    auto transform = std::make_shared<Transform>(glm::vec3(1.0f, 2.0f, 3.0f));
    e.AddComponent(transform);
    ASSERT(e.HasComponent<Transform>());
    auto retrieved = e.GetComponent<Transform>();
    ASSERT_NOT_NULL(retrieved.get());
    ASSERT_EQ(retrieved->position.x, 1.0f);
}

REGISTER_TEST(Entity_RemoveComponent) {
    Entity e(1, "TestEntity");
    auto transform = std::make_shared<Transform>();
    e.AddComponent(transform);
    ASSERT(e.HasComponent<Transform>());
    e.RemoveComponent<Transform>();
    ASSERT(!e.HasComponent<Transform>());
}

REGISTER_TEST(EventBus_Subscribe) {
    EventBus bus;
    int callCount = 0;
    bus.Subscribe(1, [&callCount](const Event&) { callCount++; });
    
    class TestEvent : public Event {
    public:
        TestEvent() : Event(1) {}
    };
    
    bus.Publish(TestEvent());
    ASSERT_EQ(callCount, 1);
}

REGISTER_TEST(Transform_GetModelMatrix) {
    Transform t;
    t.position = glm::vec3(1.0f, 2.0f, 3.0f);
    auto mat = t.GetModelMatrix();
    ASSERT_NOT_NULL(&mat);
    // Verify translation component
    ASSERT_FLOAT_EQ(mat[3][0], 1.0f);
    ASSERT_FLOAT_EQ(mat[3][1], 2.0f);
    ASSERT_FLOAT_EQ(mat[3][2], 3.0f);
}

REGISTER_TEST(Transform_GetForward) {
    Transform t;
    auto forward = t.GetForward();
    // Default forward (rotation=0) is (sin(0), 0, cos(0)) = (0, 0, 1)
    ASSERT_FLOAT_EQ(forward.z, 1.0f);
}

// ============================================================================
// Renderer Tests
// ============================================================================

REGISTER_TEST(Material_Creation) {
    Material mat("DefaultMat", "shaders/default.glsl");
    ASSERT_STR_EQ(mat.GetName(), "DefaultMat");
    ASSERT_STR_EQ(mat.GetShaderPath(), "shaders/default.glsl");
}

REGISTER_TEST(Material_Properties) {
    Material mat("TestMat", "test.glsl");
    auto& props = mat.GetProperties();
    props.metallic = 0.5f;
    props.roughness = 0.7f;
    ASSERT_FLOAT_EQ(props.metallic, 0.5f);
    ASSERT_FLOAT_EQ(props.roughness, 0.7f);
}

REGISTER_TEST(Mesh_Creation) {
    Mesh mesh("TestMesh");
    ASSERT_STR_EQ(mesh.GetName(), "TestMesh");
    ASSERT_EQ(mesh.GetVertexCount(), 0);
    ASSERT_EQ(mesh.GetIndexCount(), 0);
}

REGISTER_TEST(Mesh_SetVertices) {
    Mesh mesh("TestMesh");
    std::vector<Vertex> vertices = {
        Vertex(glm::vec3(0.0f, 0.0f, 0.0f)),
        Vertex(glm::vec3(1.0f, 0.0f, 0.0f)),
        Vertex(glm::vec3(0.0f, 1.0f, 0.0f))
    };
    mesh.SetVertices(vertices);
    ASSERT_EQ(mesh.GetVertexCount(), 3);
    ASSERT(mesh.IsDirty());
}

REGISTER_TEST(Mesh_SetIndices) {
    Mesh mesh("TestMesh");
    std::vector<uint32_t> indices = { 0, 1, 2 };
    mesh.SetIndices(indices);
    ASSERT_EQ(mesh.GetIndexCount(), 3);
    ASSERT(mesh.IsDirty());
}

// ============================================================================
// TitanEditor Tests
// ============================================================================

REGISTER_TEST(TitanEditor_CreateEntity) {
    TitanEditor editor;
    uint32_t id = editor.CreateEntity("TestEditorEntity");
    ASSERT(id != 0);
    auto entity = editor.GetEntity(id);
    ASSERT_NOT_NULL(entity);
    ASSERT_STR_EQ(entity->name, "TestEditorEntity");
}

REGISTER_TEST(TitanEditor_RemoveEntity) {
    TitanEditor editor;
    uint32_t id = editor.CreateEntity("ToRemove");
    ASSERT(editor.RemoveEntity(id));
    auto entity = editor.GetEntity(id);
    ASSERT_NULL(entity);
}

REGISTER_TEST(TitanEditor_NewMap) {
    TitanEditor editor;
    editor.CreateEntity("Entity1");
    editor.CreateEntity("Entity2");
    ASSERT(editor.NewMap("TestMap"));
    auto entity = editor.GetEntity(1);
    ASSERT_NULL(entity);
}

REGISTER_TEST(TitanEditor_SaveMapText) {
    TitanEditor editor;
    editor.CreateEntity("Entity1");
    editor.CreateEntity("Entity2");
    bool saved = editor.SaveMap("test_map.txt");
    ASSERT(saved);
}

REGISTER_TEST(TitanEditor_LoadMapText) {
    TitanEditor editor;
    editor.CreateEntity("Entity1");
    editor.SaveMap("test_load.txt");
    
    TitanEditor editor2;
    bool loaded = editor2.LoadMap("test_load.txt");
    ASSERT(loaded);
    auto entity = editor2.GetEntity(1);
    ASSERT_NOT_NULL(entity);
}

// ============================================================================
// Main Test Runner
// ============================================================================

int main() {
    std::cout << "\n";
    std::cout << " ████████╗██╗████████╗ █████╗ ███╗   ██╗███████╗███╗   ██╗ ██████╗ ██╗███╗   ██╗███████╗\n";
    std::cout << " ╚══██╔══╝██║╚══██╔══╝██╔══██╗████╗  ██║██╔════╝████╗  ██║██╔════╝ ██║████╗  ██║██╔════╝\n";
    std::cout << "    ██║   ██║   ██║   ███████║██╔██╗ ██║█████╗  ██╔██╗ ██║██║  ███╗██║██╔██╗ ██║█████╗  \n";
    std::cout << "    ██║   ██║   ██║   ██╔══██║██║╚██╗██║██╔══╝  ██║╚██╗██║██║   ██║██║██║╚██╗██║██╔══╝  \n";
    std::cout << "    ██║   ██║   ██║   ██║  ██║██║ ╚████║███████╗██║ ╚████║╚██████╔╝██║██║ ╚████║███████╗\n";
    std::cout << "    ╚═╝   ╚═╝   ╚═╝   ╚═╝  ╚═╝╚═╝  ╚═══╝╚══════╝╚═╝  ╚═══╝ ╚═════╝ ╚═╝╚═╝  ╚═══╝╚══════╝\n";
    std::cout << "                                   TEST SUITE\n";

    return TestSuite::RunAll();
}
