#pragma once

#include "core/types.h"

namespace rain
{
    struct Resource
    {
        enum class Type
        {
            TEXTURE2D,
            MESH,
            SHADER,
            TEXTURECUBEMAP
        };

        u32 id;
        Resource::Type type;
        char* name;
    };

    struct ResourceManager
    {
        Resource::Type type;
        Resource** resources;
        u32 size;
        u32 capacity;
        u32 resizeStep;

        Resource* (*Load)(const char*);
    };

    ResourceManager* CreateResourceManager(Resource::Type _type, u32 _capacity, Resource*(*_load)(const char*), u32 _resizeStep = 10);
    Resource* LoadResource(ResourceManager* _manager, const char* _path);
    Resource* GetResource(ResourceManager* _manager, u32 _id);
    Resource* GetResource(ResourceManager* _manager, const char* _name);


}