#include "gfx_camera.h"

#include <algorithm>
#include <entt.hpp>

#include "core/core.h"
#include "engine/win32/win32_input.h"
#include "engine/core/context.h"

namespace rain::engine
{
    Camera::Camera()
    {
        worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
        front = glm::vec3(0.0f, 0.0f, -1.0f);
        movement_speed = 0.0f;
        yaw = -90.0f;
        pitch = 0.0f;
    }

    void update(Camera& _camera, Transform& _transform)
    {
        glm::vec3 movement(0.0f, 0.0f, 0.0f);

        if (RAIN_INPUT->mouse_lock)
        {
            if (RAIN_INPUT->is_key_pressed(DIK_W))
            {
                movement += _camera.front * _camera.movement_speed;
            }
            if (RAIN_INPUT->is_key_pressed(DIK_S))
            {
                movement -= _camera.front * _camera.movement_speed;
            }
            if (RAIN_INPUT->is_key_pressed(DIK_A))
            {
                movement -= _camera.right * _camera.movement_speed;
            }
            if (RAIN_INPUT->is_key_pressed(DIK_D))
            {
                movement += _camera.right * _camera.movement_speed;
            }
            _transform.position += movement;

            _camera.yaw += f32(RAIN_INPUT->x_offset * 0.1f);
            _camera.pitch += f32(RAIN_INPUT->y_offset * 0.1f);
        }

        _camera.pitch = std::clamp(_camera.pitch, -89.0f, 89.0f);

        _camera.front.x = cos(glm::radians(_camera.yaw)) * cos(glm::radians(_camera.pitch));
        _camera.front.y = sin(glm::radians(_camera.pitch));
        _camera.front.z = sin(glm::radians(_camera.yaw)) * cos(glm::radians(_camera.pitch));
        _camera.front = glm::normalize(_camera.front);

        _camera.right = glm::normalize(glm::cross(_camera.front, _camera.worldUp));

        _camera.up = glm::normalize(glm::cross(_camera.right, _camera.front));

    }
}
