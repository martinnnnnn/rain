#include "gfx_camera.h"

#include <algorithm>
#include <entt.hpp>

#include "win32/win32_input.h"
#include "win32/win32_application.h"
#include "core/types.h"

namespace rain
{
    void Camera::init()
    {
        position = glm::vec3(0.0f, 7.0f, 20.0f);
        worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
        front = glm::vec3(0.0f, 0.0f, -1.0f);
        movement_speed = 0.2f;
        yaw = -90.0f;
        pitch = 0.0f;
    }

    void Camera::update()
    {
	    glm::vec3 movement(0.0f, 0.0f, 0.0f);


        if (RAIN_INPUT->mouse_lock)
        {
	        if (RAIN_INPUT->is_key_pressed(DIK_W))
	        {
		        movement += front * movement_speed;
	        }
	        if (RAIN_INPUT->is_key_pressed(DIK_S))
	        {
		        movement -= front * movement_speed;
	        }
	        if (RAIN_INPUT->is_key_pressed(DIK_A))
	        {
		        movement -= right * movement_speed;
	        }
	        if (RAIN_INPUT->is_key_pressed(DIK_D))
	        {
		        movement += right * movement_speed;
	        }
	        position += movement;

	        yaw += f32(RAIN_INPUT->x_offset * 0.1f);
	        pitch += f32(RAIN_INPUT->y_offset * 0.1f);
        }

	    //char buffer[256];
	    //sprintf_s(buffer, "(%d, %d)\n", RAIN_INPUT.x_offset, RAIN_INPUT.y_offset);
	    //OutputDebugStringA(buffer);

	    pitch = std::clamp(pitch, -89.0f, 89.0f);

	    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	    front.y = sin(glm::radians(pitch));
	    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	    front = glm::normalize(front);

	    right = glm::normalize(glm::cross(front, worldUp));

	    up = glm::normalize(glm::cross(right, front));

    }
}
