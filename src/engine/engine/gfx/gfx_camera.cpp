#include "gfx_camera.h"

#include <algorithm>
#include <entt.hpp>
#include "glm.hpp"

#include "core/core.h"
#include "engine/win32/win32_input.h"
#include "engine/core/context.h"
#include "engine/gfx/ogl/ogl_renderer.h"

namespace rain::engine
{
    Camera::Camera()
    {
        worldUp = glm::vec3{ 0.0f, 1.0f, 0.0f };
        front = glm::vec3{ 0.0f, 0.0f, -1.0f };
        movement_speed = 0.0f;
        yaw = -90.0f;
        pitch = 0.0f;
    }

    void update(Camera& _camera, core::transform& _transform)
    {
        glm::vec3 movement{ 0.0f, 0.0f, 0.0f };

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
            
            char buffer[512];
            core::string::print_to_buffer(buffer, 512, "(%.2f, %.2f, %.2f)", _transform.position.x, _transform.position.y, _transform.position.z);
            RAIN_RENDERER->draw_text_2d(buffer, 500.0f, 10.0f, 0.5f, (glm::vec4{ 0.5, 0.8f, 0.2f, 1.0f }));

            _camera.yaw += f32(RAIN_INPUT->x_frame_offset * 0.1f);
            _camera.pitch += f32(RAIN_INPUT->y_frame_offset * 0.1f);
        }

        _camera.pitch = std::clamp(_camera.pitch, -89.0f, 89.0f);

        _camera.front.x = glm::cos(glm::radians(_camera.yaw)) * glm::cos(glm::radians(_camera.pitch));
        _camera.front.y = glm::sin(glm::radians(_camera.pitch));
        _camera.front.z = glm::sin(glm::radians(_camera.yaw)) * glm::cos(glm::radians(_camera.pitch));
        _camera.front = glm::normalize(_camera.front);

        _camera.right = glm::normalize(cross(_camera.front, _camera.worldUp));

        _camera.up = glm::normalize(cross(_camera.right, _camera.front));

    }
}
