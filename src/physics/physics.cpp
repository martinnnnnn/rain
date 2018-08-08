#include "physics.h"

#include "scene/sc_scene.h"
#include "core/transform.h"

namespace rain
{
    Physics* CreateCPhysics()
    {
        Physics* physics = (Physics*)malloc(sizeof(Physics));
        physics->componentType = ComponentType::PHYSICS;
        return physics;
    }

    void InitCPhysics(Physics* _physics)
    {
        _physics->componentType = ComponentType::PHYSICS;
    }

    System* CreatePhysicsSystem(Scene* _scene)
    {
        System* physicsSystem = (System*)malloc(sizeof(System));
        physicsSystem->requirements = (1 << (u64)ComponentType::TRANSFORM) | (1 << (u64)ComponentType::PHYSICS);
        AddEntities(physicsSystem, &_scene->entities);
        return physicsSystem;
    }

    void UpdatePhysics(System* _physicsSystem, f64 deltaTime)
    {
        for (u32 i = 0; i < _physicsSystem->entities.size; ++i)
        {
            Transform* transform = (Transform*)FindComponent(_physicsSystem->entities.items[i], ComponentType::TRANSFORM);
            Physics* physics = (Physics*)FindComponent(_physicsSystem->entities.items[i], ComponentType::PHYSICS);

            transform->position = glm::vec3(
                transform->position.x + physics->direction.x * (physics->speed * deltaTime),
                transform->position.y + physics->direction.y * (physics->speed * deltaTime),
                transform->position.z + physics->direction.z * (physics->speed * deltaTime)
                );

            physics->speed -= 0.1f;
            math::Clamp<float>(physics->speed, 0.0f, physics->speed);
        }
    }
}