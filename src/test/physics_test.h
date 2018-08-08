#pragma once

#include "core/containers/vector.h"
#include "ecs/ecs.h"
#include "core/transform.h"
#include "physics/physics.h"

#include <time.h>
#include <iostream>


namespace rain
{


    Vector<Entity*>* InitEntities()
    {
        srand((u32)time(NULL));


        Vector<Entity*>* entities = CreateVector<Entity*>(5, 3);

        for (u32 i = 0; i < 5; ++i)
        {
            Transform* transform = CreateTransform();
            transform->position = glm::vec3(0, 0, 0);

            Physics* physics = CreateCPhysics();
            physics->direction = glm::vec3(rand() % 3, rand() % 3, rand() % 3);
            physics->speed = rand() % 100;

            AddItem<Entity*>(entities, CreateEntity(i, 2, 1));
            AddComponent(entities->items[i], transform);
            AddComponent(entities->items[i], physics);
        }

        return entities;
    }

    void TestPhysicsMovement()
    {
        std::cout
            << std::endl << "---------------------------------"
            << std::endl << "----- TEST PHYSICS MOVEMENT -----"
            << std::endl << "---------------------------------";
        
        Vector<Entity*>* entities = InitEntities();
        System* physicsSystem = (System*)malloc(sizeof(System));
        physicsSystem->requirements = (1 << (u64)ComponentType::TRANSFORM) | (1 << (u64)ComponentType::PHYSICS);
        InitVector<Entity*>(&physicsSystem->entities, 5, 2);
        AddEntities(physicsSystem, entities);

        //void UpdatePhysics(System* _physicsSystem, f32 deltaTime);

        f64 time = 0;
        while (time < 100)
        {
            f64 deltaTime = (rand() + 1.0) / (RAND_MAX + 2.0);
            time += deltaTime;
            UpdatePhysics(physicsSystem, deltaTime);
            for (u32 i = 0; i < physicsSystem->entities.size; ++i)
            {
                Transform* transform = (Transform*)FindComponent(physicsSystem->entities.items[i], ComponentType::TRANSFORM);
                Physics* physics = (Physics*)FindComponent(physicsSystem->entities.items[i], ComponentType::PHYSICS);

                std::cout << std::endl
                    << "Entity " << i << std::endl
                    << "Position : (" << transform->position.x << "," << transform->position.y << "," << transform->position.z << ")" << std::endl
                    << "Direction / speed : (" << physics->direction.x << "," << physics->direction.y << "," << physics->direction.z << ") / " << physics->speed << std::endl;

            }
        }
    }
}