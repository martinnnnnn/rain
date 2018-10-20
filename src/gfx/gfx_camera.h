#pragma once


#include <glm/glm.hpp>

#include "core/input.h"
#include <algorithm>

namespace rain
{
	struct Camera
	{
		glm::vec3 position;
		float movement_speed;
		float yaw;
		float pitch;
		glm::vec3 front;
		glm::vec3 right;
		glm::vec3 up;
		glm::vec3 worldUp;

		void init();
		void update();
	};

	
}