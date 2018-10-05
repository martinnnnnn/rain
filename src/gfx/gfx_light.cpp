#include "gfx/gfx_light.h"


#include "assert/rain_assert.h"

namespace rain
{

    Light* CreateLight(LightType _lightType)
    {
        Light* light = (Light*)calloc(1,sizeof(Light));
        light->componentType = ComponentType::LIGHT;
        light->lightType = _lightType;
        return light;
    }

    void InitLight(Light* _light, LightType _lightType)
    {
        _light->componentType = ComponentType::LIGHT;
        _light->lightType = _lightType;
    }


    //LightPool* CreateLightPool(u32 _capacity, u32 _resizeStep)
    //{
    //    LightPool* pool = (LightPool*)malloc(sizeof(LightPool));
    //    pool->capacity = _capacity;
    //    pool->resizeStep = _resizeStep;

    //    if (pool->capacity > 0)
    //    {
    //        pool->lights = (LightPoolElement**)calloc(pool->capacity, sizeof(LightPoolElement*));
    //        LightPoolElement* elements = (LightPoolElement*)calloc(pool->capacity, sizeof(LightPoolElement));
    //        Light* lights = (Light*)calloc(pool->capacity, sizeof(Light));

    //        for (u32 i = 0; i < pool->capacity; ++i)
    //        {
    //            pool->lights[i] = &(elements[i]);
    //            if (i == pool->capacity - 1)
    //            {
    //                pool->lights[i]->element.next = nullptr;
    //            }
    //            else
    //            {
    //                pool->lights[i]->element.next = &(elements[i+1]);
    //            }
    //        }
    //    }
    //    return pool;
    //}

    ////TODO(martin) : resize pool instead of returning nullptr when resizestep > 0
    //Light* GetLight(LightPool* _pool)
    //{
    //    RAIN_ASSERT_EQ(_pool != nullptr, "You need a initialized pool.");
    //    LightPoolElement* element = _pool->first;
    //    
    //    // no element left in the pool
    //    if (element == nullptr)
    //    {
    //        return nullptr;
    //    }

    //    _pool->first = element->element.next;
    //    element->element.light = {};
    //    element->element.light.componentType = ComponentType::LIGHT;
    //    return &(element->element.light);
    //}

    //void FreeLight(LightPool* _pool, LightPoolElement* _element)
    //{
    //    RAIN_ASSERT_EQ(_pool != nullptr, "You need a initialized pool.");

    //    for (u32 i = 0; i < _pool->capacity; ++i)
    //    {
    //        if(_pool->lights[i] == _element)
    //        {
    //            _element->element.light = {};
    //            _element->element.next = _pool->first;
    //            _pool->first = _element;
    //            _element = nullptr;
    //            break;
    //        }
    //    }
    //}
}

