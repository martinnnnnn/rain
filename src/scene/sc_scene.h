#pragma once


#include "core/containers/vector.h"


namespace rain
{
    struct System;
    struct Entity;

    struct Scene
    {
        char* name;
        char* path;
        Vector<Entity*> entities;
    };


    void ReadSceneData(const char* filePath);
    void CameraUpdatePosition(System* _cameraUpdateSystem);
    void ObjUpdatePosition(System* _objUpdateSystem);
    void TestScene();

    //void SerializeScene(Scene* _scene, const char* _path = "");
}