#pragma once

#include "core/core.h"
#include "math/math.h"

namespace rain::engine
{
    using Transform = math::Transform;
    using vec3 = math::vec3;


	struct Camera
	{
		Camera();

		float movement_speed;
		float yaw;
		float pitch;
		vec3 front;
		vec3 right;
		vec3 up;
		vec3 worldUp;
	};


    void update(Camera& _camera, Transform& _transform);
	
}