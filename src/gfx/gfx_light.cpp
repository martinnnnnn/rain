#include "gfx/gfx_light.h"




namespace rain
{
    LightPool* CreateLightPool(u32 _capacity = 100, u32 _resizeStep = 10)
    {
        LightPool* pool = (LightPool*)malloc(sizeof(LightPool));
        pool->size = 0;
        pool->capacity = _capacity;
        pool->resizeStep = _resizeStep;

        if (pool->capacity > 0)
        {
            pool->lights = (Light**)calloc(pool->capacity, sizeof(Light*));
            for (u32 i = 0; i < pool->capacity; ++i)
            {
                pool->lights[i] = (Light*)calloc(1, sizeof(Light));
                pool->lights[i]->componentType = ComponentType::LIGHT;
            }
        }
        return pool;
    }
}

