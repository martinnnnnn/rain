#pragma once

#include <Windows.h>
#include <string>
#include <unordered_map>
#include <vector>
#include <functional>
#include <cassert>

#include <glm/glm.hpp>

#include "core/singleton.h"
#include "core/types.h"


class Renderer;


template<typename Entity>
SparseSet<Entity>* intersection(SparseSet<Entity> &s)
{
    // Capacity and max value of result set 
    int iCap = min(n, s.n);
    int iMaxVal = max(s.maxValue, maxValue);

    // Create result set 
    SSet *result = new SSet(iMaxVal, iCap);

    // Find the smaller of two sets 
    // If this set is smaller 
    if (n < s.n)
    {
        // Search every element of this set in 's'. 
        // If found, add it to result 
        for (int i = 0; i < n; i++)
            if (s.search(dense[i]) != -1)
                result->insert(dense[i]);
    }
    else
    {
        // Search every element of 's' in this set. 
        // If found, add it to result 
        for (int i = 0; i < s.n; i++)
            if (search(s.dense[i]) != -1)
                result->insert(s.dense[i]);
    }

    return result;
}

SSet* SSet::setUnion(SSet &s)
{
    // Find capacity and maximum value for result 
    // set. 
    int uCap = s.n + n;
    int uMaxVal = max(s.maxValue, maxValue);

    // Create result set 
    SSet *result = new SSet(uMaxVal, uCap);

    // Traverse the first set and insert all 
    // elements of it in result. 
    for (int i = 0; i < n; i++)
        result->insert(dense[i]);

    // Traverse the second set and insert all 
    // elements of it in result (Note that sparse 
    // set doesn't insert an entry if it is already 
    // present) 
    for (int i = 0; i < s.n; i++)
        result->insert(s.dense[i]);

    return result;
}


struct Name
{
    Name() : Name("default") {}

    Name(const std::string& _name) :
        name(_name)
    {
    }

    std::string name;
};

struct Transform
{
    Transform() : Transform(glm::vec3(0.0, 0.0, 0.0)) {}

    Transform(const glm::vec3& _position) :
        position(_position)
    {

    }

    glm::vec3 position;
};

struct Physics
{
    Physics() : Physics(glm::vec3(0.0, 0.0, 0.0), 0.0) {}

    Physics(const glm::vec3& _direction, float _speed) :
        direction(_direction),
        speed(_speed)
    {
    }

    glm::vec3 direction;
    float speed;
};




class Application
{
public:
    int init(HINSTANCE _hinstance, const std::string& _config);
    void update();
    void render();
private:
    HINSTANCE hinstance;
    Renderer* renderer;
};