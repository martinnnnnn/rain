#pragma once

#include "core/core.h"

namespace rain::engine
{
    using core::math::Transform;

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


    void update(Camera& _camera, Transform& _transform);
	
}