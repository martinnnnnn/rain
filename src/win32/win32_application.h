#pragma once

#include <Windows.h>
#include <string>
#include <unordered_map>
#include <vector>
#include <functional>
#include <cassert>

#include <glm/glm.hpp>
#include <entt.hpp>


#include "core/singleton.h"
#include "core/types.h"


class Renderer;



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

    Transform(float x, float y, float z) :
        position(x, y, z)
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

    Physics(float x, float y, float z, float _speed) :
        direction(x, y, z),
        speed(_speed)
    {
    }

    glm::vec3 direction;
    float speed;
};

struct Camera;

class Application
{
public:
    int init(HINSTANCE _hinstance, const std::string& _config);
    void update();
    void render();

private:
    HINSTANCE hinstance;
    Renderer* renderer;
    entt::DefaultRegistry registry;
};