#pragma once

#include <Windows.h>



#include <string>
#include <unordered_map>
#include <vector>
#include <functional>
#include <cassert>

#include <glm/glm.hpp>
#include <entt.hpp>


#include "core/singleton.h"
#include "core/types.h"
#include "gfx/gfx_camera.h"
#include "gfx/ogl/ogl_renderer.h"
#include "core/high_resolution_clock.h"


namespace rain
{
	struct Name
	{
		Name() : Name("default") {}

		Name(const std::string& _name) :
			name(_name)
		{
		}

		std::string name;
	};

	struct Transform
	{
        Transform()
            : position(glm::vec3(0.0f))
            , previousPosition(glm::vec3(0.0f))
        {}

        glm::vec3 position;
        glm::vec3 previousPosition;
	};

	struct Physics
	{
		Physics()
            : velocity(glm::vec3(0.0f))
            , force(glm::vec3(0.0f))
            , mass(0.0f)
        {}

        glm::vec3 velocity;
        glm::vec3 force;
        float mass;
	};

	class Application
	{
	public:
		int init(HINSTANCE _hinstance, const std::string& _config);
		void shutdown();
		void update();
        void update_physics(float _deltaTime);
		void render(float _alpha);

	private:
		HINSTANCE hinstance;
		Renderer renderer;
		entt::DefaultRegistry registry;
		Camera camera;
        HighResolutionClock m_clock;
	};
}
