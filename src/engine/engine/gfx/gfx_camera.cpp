#include "gfx_camera.h"

#include <algorithm>
#include <entt.hpp>

#include "core/core.h"
#include "math/math.h"
#include "engine/win32/win32_input.h"
#include "engine/core/context.h"

namespace rain::engine
{
    Camera::Camera()
    {
        worldUp = math::vec3{ 0.0f, 1.0f, 0.0f };
        front = math::vec3{ 0.0f, 0.0f, -1.0f };
        movement_speed = 0.0f;
        yaw = -90.0f;
        pitch = 0.0f;
    }

    void update(Camera& _camera, math::transform& _transform)
    {
        math::vec3 movement{ 0.0f, 0.0f, 0.0f };

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

            _camera.yaw += f32(RAIN_INPUT->x_frame_offset * 0.1f);
            _camera.pitch += f32(RAIN_INPUT->y_frame_offset * 0.1f);
        }

        _camera.pitch = std::clamp(_camera.pitch, -89.0f, 89.0f);

        _camera.front.x = math::cos(math::to_rad(_camera.yaw)) * math::cos(math::to_rad(_camera.pitch));
        _camera.front.y = math::sin(math::to_rad(_camera.pitch));
        _camera.front.z = math::sin(math::to_rad(_camera.yaw)) * math::cos(math::to_rad(_camera.pitch));
        _camera.front = normalized(_camera.front);

        _camera.right = normalized(cross(_camera.front, _camera.worldUp));

        _camera.up = normalized(cross(_camera.right, _camera.front));

    }
}
