#include "memory/mem_resource_manager.h"


#include "stdlib.h"


namespace rain
{
    ResourceManager* CreateResourceManager(Resource::Type _type, u32 _capacity, Resource*(*_load)(const char*), u32 _resizeStep)
    {
        ResourceManager* manager = (ResourceManager*)malloc(sizeof(ResourceManager));
        manager->type = _type;
        manager->size = 0;
        manager->capacity = _capacity;
        manager->resizeStep = _resizeStep;
        manager->resources = (Resource**)calloc(_capacity, sizeof(Resource*));
        manager->Load = _load;
        return manager;
    }

    Resource* LoadResource(ResourceManager* _manager, const char* _path)
    {
        if (_manager->size == _manager->capacity)
        {
            bool wasEmpty = (_manager->capacity == 0);
            _manager->capacity += _manager->resizeStep;
            if (wasEmpty)
            {
                _manager->resources = (Resource**)calloc(_manager->capacity, sizeof(Resource*));
            }
            else
            {
                Resource** temp = (Resource**)realloc(_manager->resources, _manager->capacity * sizeof(Resource*));
                _manager->resources = temp;
            }
        }

        Resource* resource = _manager->Load(_path);
        _manager->resources[_manager->size] = resource;
        _manager->size++;
        return resource;
    }


    Resource* GetResource(ResourceManager* _manager, u32 _id)
    {
        for (u32 i = 0; i < _manager->size; ++i)
        {
            Resource* resource = *(_manager->resources + i);
            if (resource->id == _id)
            {
                return resource;
            }
        }
        return nullptr;
    }


    Resource* GetResource(ResourceManager* _manager, const char* _name)
    {
        for (u32 i = 0; i < _manager->size; ++i)
        {
            if ((_manager->resources + i)->name == _name)
            {
                return (_manager->resources + 1);
            }
        }
        return nullptr;
    }
}