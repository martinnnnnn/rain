#include "ecs/ecs_test.h"



#include <time.h>
#include <ctime>
#include <vector>
#include <iostream>

#include "ecs/ecs_component.h"
#include "ecs/ecs_entity.h"
#include "ecs_entity_container.h"
#include "ecs/ecs_system.h"


namespace rain
{

    Component* CreateTransform()
    {
        TransformTest* transform = (TransformTest*)malloc(sizeof(TransformTest));
        transform->type = Component::Type::TRANSFORM;
        transform->position = { 0, 0, 0 };
        transform->rotation = { 0, 0, 0 };
        transform->scale = { 1, 1, 1 };
        return transform;
    }

    Component* CreateScript(void(*Init)(void), void(*Update)(void), void(*Shutdown)(void))
    {
        ScriptTest* script = (ScriptTest*)malloc(sizeof(ScriptTest));
        script->type = Component::Type::SCRIPT;
        script->Init = Init;
        script->Update = Update;
        script->Shutdown = Shutdown;
        return script;
    }

    Component* CreatePhysics()
    {
        PhysicsTest* physics = (PhysicsTest*)malloc(sizeof(PhysicsTest));
        physics->type = Component::Type::PHYSICS;
        physics->friction = 0;
        physics->grounded = false;
        physics->weight = 0;
        physics->direction = { 0, 0, 0 };
        physics->speed = 0;
        return physics;
    }

    Component* CreateModel()
    {
        ModelTest* model = (ModelTest*)malloc(sizeof(ModelTest));
        model->type = Component::Type::MODEL;
        model->indices = nullptr;
        model->vertices = nullptr;
        model->size = 0;
        return model;
    }

    void PrintBit(size_t const size, void const * const ptr)
    {
        unsigned char *b = (unsigned char*)ptr;
        unsigned char byte;
        int i, j;

        for (i = size - 1; i >= 0; i--)
        {
            for (j = 7; j >= 0; j--)
            {
                byte = (b[i] >> j) & 1;
                printf("%u", byte);
            }
        }
        puts("");
    }

    void EntityTests()
    {
        // ENTITY TEST CREATION
        Entity* ent1 = CreateEntity(1, "ent1", 0);
        Entity* ent2 = CreateEntity(2, "ent2", 1);

        // ENTITY TEST ADD COMPONENT
        Component* comp1 = (TransformTest*)calloc(1, sizeof(TransformTest));
        comp1->type = Component::Type::TRANSFORM;
        TransformTest* comp2 = (TransformTest*)malloc(sizeof(TransformTest));
        comp2->type = Component::Type::TRANSFORM;
        comp2->scale = { 1, 1, 1 };
        comp2->position = { 0, 0, 0 };
        comp2->rotation = { 0, 90, 0 };
        AddComponent(ent1, comp1);
        AddComponent(ent2, comp2);

        std::cout << "Entity creation : "
            << std::endl << "-> id : " << ent1->id
            << std::endl << "-> name : " << ent1->name
            << std::endl << "-> flags : " << std::endl;
        PrintBit(sizeof(ent1->flags), &ent1->flags);
        std::cout << "-> components ? : " << (ent1->components != nullptr)
            << std::endl << "-> size : " << ent1->size
            << std::endl << "-> capacity : " << ent1->capacity
            << std::endl << std::endl;

        std::cout << "Entity creation : "
            << std::endl << "-> id : " << ent2->id
            << std::endl << "-> name : " << ent2->name
            << std::endl << "-> flags : " << std::endl;
        PrintBit(sizeof(ent2->flags), &ent2->flags);
        std::cout << "-> components ? : " << (ent2->components != nullptr)
            << std::endl << "-> size : " << ent2->size
            << std::endl << "-> capacity : " << ent2->capacity
            << std::endl << std::endl;




        // ENTITY TEST FIND COMPONENT
        TransformTest* tran1 = (TransformTest*)FindComponent(ent1, Component::Type::TRANSFORM);
        if (tran1)
        {
            std::cout << std::endl << "found transform in ent1 : "
                << std::endl << "(" << tran1->position.x << "," << tran1->position.y << "," << tran1->position.z << ")"
                << std::endl << "(" << tran1->scale.x << "," << tran1->scale.y << "," << tran1->scale.z << ")"
                << std::endl << "(" << tran1->rotation.x << "," << tran1->rotation.y << "," << tran1->rotation.z << ")";
        }
        else
        {
            std::cout << std::endl << "ERROR :: FINDCOMPONENT transform in ent1 failed" << std::endl;
        }

        TransformTest* tran2 = (TransformTest*)FindComponent(ent2, Component::Type::TRANSFORM);
        if (tran2)
        {
            std::cout << std::endl << "found transform in ent2 : "
                << std::endl << "(" << tran2->position.x << "," << tran2->position.y << "," << tran2->position.z << ")"
                << std::endl << "(" << tran2->scale.x << "," << tran2->scale.y << "," << tran2->scale.z << ")"
                << std::endl << "(" << tran2->rotation.x << "," << tran2->rotation.y << "," << tran2->rotation.z << ")";
        }
        else
        {
            std::cout << std::endl << "ERROR :: FINDCOMPONENT transform in ent2 failed" << std::endl;
        }

        // ENTITY TEST REMOVE COMPONENT
        PhysicsTest* comp3 = (PhysicsTest*)calloc(1, sizeof(PhysicsTest));
        comp3->type = Component::Type::PHYSICS;
        comp3->grounded = true;
        comp3->weight = 127;
        comp3->friction = 42;

        AddComponent(ent1, comp3);
        PhysicsTest* phy1 = (PhysicsTest*)FindComponent(ent1, Component::Type::PHYSICS);
        if (phy1)
        {
            std::cout << std::endl << "found physics in ent1 : "
                << std::endl << "friction : " << phy1->friction << ", grounded : " << (phy1->grounded ? "true" : "false") << ", weight :" << phy1->weight << std::endl;
        }
        else
        {
            std::cout << std::endl << "ERROR :: FINDCOMPONENT physics in ent1 failed" << std::endl;
        }
        RemoveComponent(ent1, Component::Type::PHYSICS);
        PhysicsTest* phy2 = (PhysicsTest*)FindComponent(ent1, Component::Type::PHYSICS);
        if (phy2)
        {
            std::cout << std::endl << "ERROR :: physics still in ent1" << std::endl;
        }
        else
        {
            std::cout << std::endl << "removed worked ! " << std::endl;
        }

        // ENTITY TEST FITS REQUIREMENTS
        comp3 = (PhysicsTest*)calloc(1, sizeof(PhysicsTest));
        comp3->type = Component::Type::PHYSICS;
        comp3->grounded = true;
        comp3->weight = 127;
        comp3->friction = 42;
        AddComponent(ent1, comp3);
        u64 mask1 = (1 << (u64)Component::Type::TRANSFORM) | (1 << (u64)Component::Type::PHYSICS);
        u64 mask2 = (1 << (u64)Component::Type::TRANSFORM) | (1 << (u64)Component::Type::MODEL);
        u64 mask3 = (1 << (u64)Component::Type::TRANSFORM) | (1 << (u64)Component::Type::PHYSICS) | (1 << (u64)Component::Type::MODEL);
        u64 mask4 = (1 << (u64)Component::Type::TRANSFORM);
        u64 mask5 = 0;

        bool fits1 = FitsRequirements(ent1, mask1);
        bool fits2 = FitsRequirements(ent1, mask2);
        bool fits3 = FitsRequirements(ent1, mask3);
        bool fits4 = FitsRequirements(ent1, mask4);
        bool fits5 = FitsRequirements(ent1, mask5);

        std::cout << std::endl << "1 (true) : " << (fits1 ? "true" : "false")
            << std::endl << "2 (false) : " << (fits2 ? "true" : "false")
            << std::endl << "3 (false) : " << (fits3 ? "true" : "false")
            << std::endl << "4 (true) : " << (fits4 ? "true" : "false")
            << std::endl << "5 (true) : " << (fits5 ? "true" : "false") << std::endl;

        std::cout << std::endl;
    }

    void EntityContainerTest()
    {
        // CONTAINER CREATION
        EntityContainer* container = CreateEntityContainer(10, 7);
        // ADDING ENTITIES
        std::vector<Entity*> tempvec;

        for (u32 i = 0; i < 20; i++)
        {
            Entity* ent = CreateEntity(i, "ent");
            tempvec.push_back(ent);
            AddEntity(container, ent);
        }

        std::cout << std::endl << "after add : " << container->size << "," << container->capacity << std::endl;

        // REMOVING ENTITIES
        for (u32 i = 0; i < 18;)
        {
            RemoveEntity(container, tempvec[i]);
            i += 2;
        }
        std::cout << std::endl << "after remove : " << container->size << "," << container->capacity << std::endl;
    }

    void SystemTest()
    {
        srand(time(NULL));
        // CONTAINER CREATION
        EntityContainer* container1 = CreateEntityContainer(10000000, 100, 10000000);
        for (int i = 0; i < 10; i++)
        {
            AddComponent(container1->entities[i], CreateTransform());
            AddComponent(container1->entities[i], CreatePhysics());
            PhysicsTest* physics = (PhysicsTest*)FindComponent(container1->entities[i], Component::Type::PHYSICS);
            if (physics)
            {
                int r = rand() % 100;
                physics->weight = rand() % 100;
                physics->friction = rand() % 200 + 100;
                physics->grounded = (rand() % 2) ? true : false;
                physics->direction = { (float)(rand() % 10), (float)(rand() % 10), (float)(rand() % 10) };
                physics->speed = rand() % 5;
            }
        }

        // SYSTEM CREATION
        u64 requirements = (1 << (u32)Component::Type::PHYSICS);
        System* system1 = CreateSystem(requirements, 100, 100);

        // SYSTEM ADD
        CheckForAddEntities(system1, container1->entities, container1->size);

        // SYSTEM UPDATE
    }
    //std::clock_t start;
    //double duration;
    //
    //start = std::clock();
    //duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;
    //
    //std::cout << "duration 2: " << duration << '\n';
}