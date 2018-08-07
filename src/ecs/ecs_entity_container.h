//#pragma once
//
//
//#include "ecs/ecs_entity.h"
//#include "core/types.h"
//
//namespace rain
//{
//    struct EntityContainer
//    {
//        Entity** entities;
//        u32 size;
//        u32 capacity;
//        u32 resizeStep;
//    };
//
//    EntityContainer* CreateEntityContainer(u32 _capacity, u32 _resizeStep = 10, u32 _size = 0);
//    void AddEntity(EntityContainer* _container, Entity* _entity);
//    void RemoveEntity(EntityContainer* _container, Entity* _ent);
//}