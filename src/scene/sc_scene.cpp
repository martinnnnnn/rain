#include "scene/sc_scene.h"



#include "core/types.h"
#include "ecs/ecs.h"
#include "gfx/gfx_camera.h"

#include <stdio.h>

namespace rain
{
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

        objUpdateRequirements = (1 << (u64)ComponentType::TRANSFORM) | (1 << (u64)ComponentType::PHYSICS) | (1 << (u64)ComponentType::SCRIPT);
        System* cameraUpdate = CreateSystem(objUpdateRequirements, 100, 100);
    }
    


    //void SerializeScene(Scene* _scene, const char* _path)
    //{
    //    FILE *fp = nullptr;
    //    errno_t error = fopen_s(&fp, _path, "w");
    //    if (error != 0)
    //    {
    //        printf("File not created, errno = %d\n", error);
    //        return;
    //    }
    //    fprintf(fp, "Hello, there.\n"); 
    //    fprintf(fp, "I said hello.\n");
    //    fprintf(fp, "Come on !\n");
    //    fprintf(fp, "qslmdfkjsdf\n");
    //    fprintf(fp, "Hello\n");
    //    fprintf(fp, "Hello???\n");
    //    
    //    

    //    //for (u32 i = 0; i < _scene->entityContainer->size; ++i)
    //    //{
    //    //    Entity* entity = _scene->entityContainer->entities[i];
    //    //    fprintf(fp, "---");
    //    //}

    //    fclose(fp);
    //    return;
    //}
}