#pragma once

#include "core/core.h"
#include "glm.hpp"

namespace rain::engine
{

	struct Camera
	{
		Camera();

		float movement_speed;
		float yaw;
		float pitch;
		glm::vec3 front;
		glm::vec3 right;
		glm::vec3 up;
		glm::vec3 worldUp;
	};


    void update(Camera& _camera, core::transform& _transform);
	
}