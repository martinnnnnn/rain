#include "gfx/gfx_camera.h"

#include "rain.h"
#include "input/input.h"
#include "core/transform.h"

namespace rain
{
    Camera* GetDefaultCamera(Game* _game)
    {
        Camera* camera = new Camera();
        camera->transform = CreateTransform();
        camera->game = _game;
        camera->movementSpeed = 20;
        camera->worldUp = glm::vec3(0.0, 1.0, 0.0);;
        camera->front = glm::vec3(0.0, 0.0, -1.0f);
        camera->yaw = 0.0f;
        camera->pitch = 0.0f;
        UpdateCamera(*camera);
        UpdateProjectionMatrix(*camera, glm::vec2(camera->game->gfxContext.width, camera->game->gfxContext.height));
        return camera;
    }


    void UpdateCamera(Camera& _camera)
    {
        float velocity = _camera.movementSpeed * _camera.game->deltaTime;
        glm::vec3 movement(0.0f, 0.0f, 0.0f);
        glm::vec3 front = _camera.front;
        glm::vec3 right = _camera.right;
        Input* input = _camera.game->input;

        if (IsKeyPressed(input, GLFW_KEY_W))
        {
            movement += front * velocity;
        }
        if (IsKeyPressed(input, GLFW_KEY_S))
        {
            movement -= front * velocity;
        }
        if (IsKeyPressed(input, GLFW_KEY_A))
        {
            movement -= right * velocity;
        }
        if (IsKeyPressed(input, GLFW_KEY_D))
        {
            movement += right * velocity;
        }
        Translate(_camera.transform, movement);

        glm::vec2 offset = input->mouseOffset;

        offset.x *= 0.1f;
        offset.y *= 0.1f;

        _camera.yaw += offset.x;
        _camera.pitch += offset.y;

        if (_camera.pitch > 89.0f)
            _camera.pitch = 89.0f;
        if (_camera.pitch < -89.0f)
            _camera.pitch = -89.0f;

        SetRotation(_camera.transform, glm::vec3(0, -1, 0), _camera.yaw);
        Rotate(_camera.transform, glm::vec3(-1, 0, 0), _camera.pitch);

        glm::mat4 rotMat = glm::mat4_cast(_camera.transform->rotation);

        _camera.front = glm::normalize(rotMat[2]);

        _camera.right = glm::normalize(glm::cross(_camera.front, _camera.worldUp));
        _camera.up = glm::normalize(glm::cross(_camera.right, _camera.front));
    }

    glm::mat4 GetViewMatrix(Camera& _camera)
    {
        return glm::lookAt(_camera.transform->position, _camera.transform->position + _camera.front, _camera.up);
    }

    glm::mat4 GetProjectionMatrix(Camera& _camera)
    {
        return _camera.projectionMatrix;
    }

    void UpdateProjectionMatrix(Camera& _camera, const glm::vec2& _screen)
    {
        UpdateProjectionMatrix(_camera, (float)_screen.x, (float)_screen.y);
    }

    void UpdateProjectionMatrix(Camera& _camera, float width, float height)
    {
        _camera.projectionMatrix = glm::perspective(glm::radians(45.0f), width / height, 0.1f, 10000.0f);
    }
}