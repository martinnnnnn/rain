//#include "entity.h"
//
//#include <iostream>
//
//
//namespace rain
//{
//	/* Entity* CreateEntity(int _id, char* _name = "", uint64_t _flags = 0)
//	 {
//		 Entity* entity = (Entity*)malloc(sizeof(Entity));
//		 bzero(entity, sizeof(Entity));
//
//		 entity->id = _id;
//		 entity->name = _name;
//		 entity->flags = _flags;
//
//		 for (uint64_t i = 0; i < (uint64_t)Component::Type::MAX_COUNT; ++i)
//		 {
//			 if (GetBits(_flags, i);
//		 }
//
//		 if (_flags)
//		 {
//			 CreateComponent<Transform>(entity);
//		 }
//
//	 }*/
//
//
//    Entity* GetFreeEntity(EntityContainer* _container)
//    {
//        Entity* entity = (Entity*)malloc(sizeof(Entity));
//        bzero(entity, sizeof(Entity));
//		return entity;
//    }
//
//    void ReleaseEntity(EntityContainer* _container, Entity* _entity)
//    {
//
//    }
//
//
//    Entity* GetEntity(EntityContainer* _container, const std::string& _name)
//    {
//        for (size_t i = 0; i < _container->entities.size(); ++i)
//        {
//            if (_container->entities[i]->name == _name)
//            {
//                return _container->entities[i];
//            }
//        }
//        return nullptr;
//    }
//
//    Entity* GetEntity(EntityContainer* _container, const int _id)
//    {
//        for (size_t i = 0; i < _container->entities.size(); ++i)
//        {
//            if (_container->entities[i]->id == _id)
//            {
//                return _container->entities[i];
//            }
//        }
//        return nullptr;
//    }
//}