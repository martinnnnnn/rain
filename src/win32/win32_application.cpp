#include "win32_application.h"

//#include <entt.hpp>
#include <windows.h>
#include <time.h>
#include <stdlib.h>
#include <algorithm>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

#include "win32_window.h"
#include "win32/win32_input.h"
#include "win32/win32_window.h"
#include "core/event.h"
#include "core/data_indexer.h"
#include "math/transform.h"
#include "gfx/ogl/ogl_renderer.h"
#include "physics/physics.h"
#include "physics/rigid_body.h"
#include "physics/collision.h"
#include "game/world.h"
#include "core/data_indexer.h"

using namespace rain;


int Application::init(HINSTANCE _hinstance, const std::string& _config)
{
    hinstance = _hinstance;

    // INIT CONFIG
    data = new Data();
    data->init(File::get_exe_path() + "/config.rain");

	// INIT WINDOW
    window = new Window();
	window->init(hinstance, 800, 600, 0);

    // INIT RENDERER
    renderer = new Renderer();
	renderer->init();

    // SHOW WINDOW
	window->show();

	// INIT INPUT
    input = new Input();
	input->init();

    // INIT CLOCK
    m_clock.reset();

    world.init(RAIN_DATA->root + "/world/world_1.rain");

    return 0;
}


void Application::update()
{
    static const double dt = 0.01;

    static double currentTime = m_clock.get_total_seconds();
    static double accumulator = 0.0;

    input->update();
    renderer->update_camera();
    m_clock.tick();

    double newTime = m_clock.get_total_seconds();
    double frameTime = newTime - currentTime;
    if (frameTime > 0.25)
    {
        frameTime = 0.25;
    }
    currentTime = newTime;
    accumulator += frameTime;

    while (accumulator >= dt)
    {

        update_physics((float)dt);
        accumulator -= dt;
    }

    const double alpha = accumulator / dt;
    render(float(alpha));
    if (window->is_initialized())
    {
        window->present();
    }
}

void Application::update_physics(float _deltaTime)
{
    // applying springs
    auto spring_view = world.registry.view<Spring>();
    for (auto entity : spring_view) 
    {
        Spring& spring = spring_view.get(entity);
        RigidBody& body = world.registry.get<RigidBody>(spring.entity);
        Transform& transform = world.registry.get<Transform>(spring.entity);
        Physics::apply_spring(spring, transform, body);
    }

    // applying ropes
    auto rope_view = world.registry.view<SpringRope>();
    for (auto entity : rope_view)
    {
        SpringRope& rope = rope_view.get(entity);
        RigidBody& bodyA = world.registry.get<RigidBody>(rope.entityA);
        Transform& transformA = world.registry.get<Transform>(rope.entityA);
        RigidBody& bodyB = world.registry.get<RigidBody>(rope.entityB);
        Transform& transformB = world.registry.get<Transform>(rope.entityB);
        Physics::apply_spring(rope, transformA, bodyA, transformB, bodyB);
    }

    // updating physics
    auto physics_view = world.registry.view<RigidBody, Transform>();
    for (auto entity : physics_view)
    {
        RigidBody& body = physics_view.get<RigidBody>(entity);
        Physics::apply_gravity(body);
        Physics::update(body, physics_view.get<Transform>(entity), _deltaTime);
    }

    // updating collision
    auto view = world.registry.view<RigidBody, BoundingSphere, Transform>();
    for (auto entity1 : view)
    {
        RigidBody& body1 = view.get<RigidBody>(entity1);
        BoundingSphere& bound1 = view.get<BoundingSphere>(entity1);
        Transform& transform1 = view.get<Transform>(entity1);
        for (auto entity2 : view)
        {
            if (entity1 == entity2)
            {
                break;
            }

            RigidBody& body2 = view.get<RigidBody>(entity2);
            BoundingSphere& bound2 = view.get<BoundingSphere>(entity2);
            Transform& transform2 = view.get<Transform>(entity2);

            bool collide = detect_collision(body1, bound1, transform1, body2, bound2, transform2);
			if (collide)
			{
				collision_response(body1, transform1, body2, transform2);
			}
        }
    }
}


void Application::shutdown()
{
    input->shutdown();
}


void Application::render(float _alpha)
{
    renderer->clear();
    renderer->render_coord_view(glm::vec3(0.0f, 0.0f, 0.0f));
    //renderer.set_view_matrix(camera.position, glm::radians(camera.pitch), glm::radians(camera.yaw));
    //renderer->set_view_matrix(camera->position, camera->position + camera->front, camera->up);

    auto view = world.registry.view<Transform>();

    for (auto entity : view)
    {
        Transform& transform = view.get(entity);
        glm::vec3 position = transform.position * _alpha + transform.previousPosition * (1.0f - _alpha);
        glm::quat orientation = transform.orientation * _alpha + transform.previousOrientation * (1.0f - _alpha);

		renderer->render_sphere(position, orientation);
    }
}


int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE hprevinstance, LPSTR lpcmdline, int nshowcmd)
{
    RAIN_APPLICATION.init(hinstance, "");

    MSG msg;
    bool quit = false;
    while (!quit)
    {
        if (PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
                quit = true;

            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        RAIN_APPLICATION.update();

        if (GetAsyncKeyState(VK_ESCAPE))
        {
            RAIN_WINDOW->shutdown();
        }
    }

    return msg.lParam;
}
