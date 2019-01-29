#pragma once

#include "core/core.h"
#include "math/math.h"

namespace rain::engine
{

	struct Camera
	{
		Camera();

		float movement_speed;
		float yaw;
		float pitch;
		math::vec3 front;
		math::vec3 right;
		math::vec3 up;
		math::vec3 worldUp;
	};


    void update(Camera& _camera, math::Transform& _transform);
	
}