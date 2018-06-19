#pragma once

#include "ecs/ecs_component.h"
#include "ecs/ecs_system.h"

namespace rain
{
    

    struct TransformTest : public Component
    {
        glm::vec3 position;
        glm::vec3 rotation;
        glm::vec3 scale;
    };


    struct ScriptTest : Component
    {
        void(*Init)(void);
        void(*Update)(void);
        void(*Shutdown)(void);
    };


    struct PhysicsTest : Component
    {
        float weight;
        float friction;
        bool grounded;
        glm::vec3 direction;
        float speed;
    };


    struct ModelTest : Component
    {
        glm::vec3* vertices;
        glm::vec3* indices;
        u32 size;
    };
    
    Component* CreateTransformTest();
    Component* CreatePhysicsTest();
    Component* CreateScriptTest(void(*Init)(void), void(*Update)(void), void(*Shutdown)(void));
    Component* CreateModelTest();


    void PrintBit(size_t const size, void const * const ptr);
    void PrintTransformTest(TransformTest* _transform);

    void SystemPhysicsUpdate(System* _system);

    // TODO(martin) : make real tests
    void EntityTests();
    void EntityContainerTest();
    void SystemTest();
}