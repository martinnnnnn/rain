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

//float velocity = _camera.movementSpeed * _camera.game->deltaTime;
//glm::vec3 movement(0.0f, 0.0f, 0.0f);
//glm::vec3 front = _camera.front;
//glm::vec3 right = _camera.right;
//Input* input = _camera.game->input;
//
//if (IsKeyPressed(input, GLFW_KEY_W))
//{
//    movement += front * velocity;
//}
//if (IsKeyPressed(input, GLFW_KEY_S))
//{
//    movement -= front * velocity;
//}
//if (IsKeyPressed(input, GLFW_KEY_A))
//{
//    movement -= right * velocity;
//}
//if (IsKeyPressed(input, GLFW_KEY_D))
//{
//    movement += right * velocity;
//}
//Translate(_camera.transform, movement);
//

//glm::vec2 offset = input->mouseOffset;
//
//offset.x *= 0.1f;
//offset.y *= 0.1f;
//
//_camera.yaw += offset.x;
//_camera.pitch += offset.y;
//
//if (_camera.pitch > 89.0f)
//_camera.pitch = 89.0f;
//if (_camera.pitch < -89.0f)
//    _camera.pitch = -89.0f;
//
//SetRotation(_camera.transform, glm::vec3(0, -1, 0), _camera.yaw);
//Rotate(_camera.transform, glm::vec3(-1, 0, 0), _camera.pitch);
//
//glm::mat4 rotMat = glm::mat4_cast(_camera.transform->rotation);
//
//_camera.front = glm::normalize(rotMat[2]);
//
//_camera.right = glm::normalize(glm::cross(_camera.front, _camera.worldUp));
//_camera.up = glm::normalize(glm::cross(_camera.right, _camera.front));




struct Camera
{
    glm::vec3 position;
    float movement_speed;
    float yaw;
    float pitch;
    glm::vec3 front;
    glm::vec3 right;
};


class Application
{
public:
    int init(HINSTANCE _hinstance, const std::string& _config);
    void update();
    void render();

    void update_camera();

private:
    HINSTANCE hinstance;
    Renderer* renderer;
    entt::DefaultRegistry registry;
    Camera& camera;
};