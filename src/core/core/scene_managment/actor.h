#pragma once

#include <vector>

#include "core/types.h"

namespace rain::core
{
    class Component;

    struct Actor
    {
        using Components = std::vector<Component*>;
        using Children = std::vector<Actor*>;

        Actor(Actor* parent = nullptr);
        ~Actor();

        void init();
        void tick();
        void shutdown();

        void add(Component* component);
        void remove(Component* component);
        void remove_component(u32 index);
        bool contains(Component* component);

        void add(Actor* child);
        void remove(Actor* child);
        void remove_child(u32 index);
        bool contains(Actor* child);

        template<typename T>
        T* get_component();

        Actor*      parent;
        Children    children;
        Components  components;
    };

    template<typename T>
    T* Actor::get_component()
    {
        static_assert(std::is_base_of<Component, T>::value, "You can only acquire objects from Component derived classes");

        for (int i = 0; i < components.size(); ++i)
        {
            if (components[i]->is_exactly(T::TYPE))
            {
                return rtti_static_cast<T>(components[i]);
            }
        }
        return nullptr;
    }
}
