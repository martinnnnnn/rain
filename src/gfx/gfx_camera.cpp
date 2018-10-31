#include "gfx_camera.h"


using namespace rain;

void Camera::init()
{
	position = glm::vec3(0.0f, 0.0f, 10.0f);
	worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
	front = glm::vec3(0.0f, 0.0f, -1.0f);
	movement_speed = 0.1f;
	yaw = -90.0f;
	pitch = 0.0f;
}

void Camera::update()
{
	glm::vec3 movement(0.0f, 0.0f, 0.0f);

	if (RAIN_INPUT.is_key_pressed(DIK_W))
	{
		movement += front * movement_speed;
	}
	if (RAIN_INPUT.is_key_pressed(DIK_S))
	{
		movement -= front * movement_speed;
	}
	if (RAIN_INPUT.is_key_pressed(DIK_A))
	{
		movement -= right * movement_speed;
	}
	if (RAIN_INPUT.is_key_pressed(DIK_D))
	{
		movement += right * movement_speed;
	}
	position += movement;

	yaw += (float)RAIN_INPUT.x_offset * 0.1f;
	pitch += (float)RAIN_INPUT.y_offset * 0.1f;

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