#include "actor.h"

#include "component.h"

namespace rain::core
{
    Actor::Actor(Actor* parent)
    {
        this->parent = parent;
    }

    Actor::~Actor()
    {
        for (int i = 0; i < components.size(); ++i)
        {
            delete components[i];
        }

        for (int i = 0; i < children.size(); ++i)
        {
            delete children[i];
        }
    }

    void Actor::init()
    {

    }

    void Actor::tick()
    {
        for (int i = 0; i < components.size(); ++i)
        {
            components[i]->tick();
        }

        for (int i = 0; i < children.size(); ++i)
        {
            children[i]->tick();
        }
    }

    void Actor::shutdown()
    {
        for (int i = 0; i < components.size(); ++i)
        {
            components[i]->shutdown();
        }

        for (int i = 0; i < children.size(); ++i)
        {
            children[i]->shutdown();
        }
    }

    void Actor::add(Component* component)
    {
        if (!contains(component))
        {
            components.push_back(component);
            component->actor = this;
        }
    }

    void Actor::remove(Component* component)
    {
        for (u32 i = 0; i < components.size(); ++i)
        {
            if (components[i] == component)
            {
                remove_component(i);
                break;
            }
        }
    }

    void Actor::remove_component(u32 index)
    {
        components[index]->shutdown();
        delete components[index];
        components.erase(components.begin() + index);
    }

    bool Actor::contains(Component* component) 
    {
        for (u32 i = 0; i < components.size(); ++i)
        {
            if (components[i] == component)
            {
                return true;
            }
        }
        return false;
    }

    void Actor::add(Actor* child)
    {
        if (!contains(child))
        {
            children.push_back(child);
            child->parent = this;
        }
    }

    void Actor::remove(Actor* child)
    {
        i32 to_remove = -1;
        for (u32 i = 0; i < children.size(); ++i)
        {
            if (children[i] == child)
            {
                to_remove = i;
                break;
            }
        }

        if (to_remove != -1)
        {
            children[to_remove]->shutdown();
            delete children[to_remove];
            children.erase(children.begin() + to_remove);
        }
    }

    void Actor::remove_child(u32 index)
    {
        children[index]->shutdown();
        delete children[index];
        children.erase(children.begin() + index);
    }

    bool Actor::contains(Actor* child)
    {
        for (u32 i = 0; i < children.size(); ++i)
        {
            if (children[i] == child)
            {
                return true;
            }
        }
        return false;
    }
}




