//#pragma once
//
//#include "core/memory.h"
//
//namespace rain
//{
//    template<typename T>
//    bool AddComponent(Entity* _entity, T* _component)
//    {
//        static_assert(std::is_base_of<Component, T>::value, "You can only add objects from Component derived structs");
//        uint64_t flag = (uint64_t)_component->type;
//        if (GetBits(_entity->flags, flag))
//        {
//            return false;
//        }
//        TurnOnBits(_entity->flags, flag);
//        _entity->components.push_back(static_cast<Component*>(_component));
//        return true;
//    }
//
//    template<typename T>
//    T* GetComponent(Entity* _entity, uint64_t _componentFlag)
//    {
//        if (GetBits(_entity->flags, _componentFlag))
//        {
//            for (unsigned int i = 0; i < _entity->components.size(); ++i)
//            {
//                if ((uint64_t)_entity->components[i]->type == _componentFlag)
//                {
//                    return static_cast<T*>(_entity->components[i]);
//                }
//            }
//        }
//        return nullptr;
//    }
//
//    template<typename T>
//    T* CreateComponent(Entity* _entity)
//    {
//        static_assert(std::is_base_of<Component, T>::value, "You can only create objects from Component derived structs");
//
//        T* newComponent = (T*)malloc(sizeof(T));
//        bzero(newComponent, sizeof(T));
//        //T* newComponent = new T();
//        if (GetBit(_entity->flags, (uint64_t)newComponent->type))
//        {
//            delete(newComponent);
//            return nullptr;
//        }
//        TurnOnBit(_entity->flags, (uint64_t)newComponent->type);
//        _entity->components.push_back(newComponent);
//        return newComponent;
//    }
//}