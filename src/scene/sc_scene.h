#pragma once


#include "ecs/ecs.h"
#include "gfx/gfx_camera.h"

namespace rain
{
    struct Scene
    {
        char* name;
        char* path;
        Vector<Entity*> entities;
    };


    void ReadSceneData(const char* filePath)
    {
        // adds entities in scene
    }


    void CameraUpdatePosition(System* _cameraUpdateSystem)
    {

    }

    void ObjUpdatePosition(System* _objUpdateSystem)
    {
        for (u32 i = 0; i < _objUpdateSystem->entities.size; ++i)
        {
            //_objUpdateSystem->entities[i]
        }
        
    }

    void TestScene()
    {
        Scene scene;
        scene.name = "Test Scene";
        scene.path = "D:/this/is/a/path/to/a/scene.sc";

        // camera entity creation
        Entity camera;
        InitEntity(&camera, 0);

        // transform component
        Transform* transform = CreateTransform();
        AddComponent(&camera, transform);

        // camera component


        AddItem<Entity*>(&scene.entities, &camera);


        // systems creation
        u64 objUpdateRequirements = (1 << (u64)ComponentType::TRANSFORM) | (1 << (u64)ComponentType::PHYSICS);
        System* objUpdate = CreateSystem(objUpdateRequirements, 100, 100);

        u64 objUpdateRequirements = (1 << (u64)ComponentType::TRANSFORM) | (1 << (u64)ComponentType::PHYSICS) | (1 << (u64)ComponentType::SCRIPT);
        System* cameraUpdate = CreateSystem(objUpdateRequirements, 100, 100);
    }

    //void SerializeScene(Scene* _scene, const char* _path = "");
}