#include "gfx/gfx_light.h"


#include "assert/rain_assert.h"

namespace rain
{
    LightPool* CreateLightPool(u32 _capacity, u32 _resizeStep)
    {
        RAIN_ASSERT_EQ(_capacity >= 0, "You can't create a pool of negative size.");
        RAIN_ASSERT_EQ(_resizeStep > 0, "The resize step must be > 0.");

        LightPool* pool = (LightPool*)malloc(sizeof(LightPool));
        pool->size = 0;
        pool->capacity = _capacity;
        pool->resizeStep = _resizeStep;

        if (pool->capacity > 0)
        {
            pool->lights = (Light**)calloc(pool->capacity, sizeof(Light*));
            Light* lights = (Light*)calloc(pool->capacity, sizeof(Light));
            for (u32 i = 0; i < pool->capacity; ++i)
            {
                pool->lights[i] = &(lights[i]);
                pool->lights[i]->componentType = ComponentType::LIGHT;
            }
        }
        return pool;
    }
}

