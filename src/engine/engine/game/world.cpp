#include "world.h"

#include <cassert>
#include <algorithm>
#include <thread>
#include <new>

#include "engine/core/context.h"
#include "engine/win32/win32_application.h"
#include "engine/core/json_reader.h"
#include "engine/gfx/ogl/ogl_renderer.h"
#include "engine/physics/collision.h"
#include "engine/physics/rigid_body.h"
#include "engine/physics/physics.h"
#include "engine/physics/spring.h"
#include "engine/core/profiler.h"
#include "engine/data/data_system.h"
#include "engine/ui/text_field.h"
#include "engine/ui/text_list.h"
#include "glm.hpp"
#include "engine/network/client.h"
#include "engine/voxel/voxel.h"
#include "engine/win32/win32_input.h"

namespace rain::engine
{
    void World::init(const std::string& _path)
    {
        RAIN_PROFILE("World Initialization");

        {
            //RAIN_PROFILE("Chunk init");
            //vmap = new voxel_map();s
            //engine::init(vmap);
        }

        {
            RAIN_PROFILE("Transvoxel");

            //voxmap.min_x = 0;
            voxmap.min_y = 0;
            //voxmap.min_z = 0;
            //voxmap.max_x = 5;
            voxmap.max_y = 1;
            //voxmap.max_z = 1;
            voxel::init_map(&voxmap, glm::vec3{ -0.f, -0.f, -0.f }, 3, RAIN_CONFIG->data_root + "/../runtime_data");
        }

        std::vector<actor*> view;

        file.open(_path);
        json_reader::read_world(file.read(), *this);

        sg.get_view<ui::text_field, ui::text_list>(view, true);
        for (auto chat : view)
        {
            ui::text_field* field = chat->components.get<ui::text_field>();
            ui::text_list* list = chat->components.get<ui::text_list>();
            field->on_validate.connect_member(RAIN_CONTEXT->app, &application::send_to_network);
            RAIN_MESSAGING->subscribe<network::INGAME_CHAT_INC>(std::bind(&ui::text_list::add_line, list, std::placeholders::_1));
            break;
        }

        //retrieving main camera
        sg.get_view<core::transform, Camera>(view, true);
        for (auto act : view)
        {
            main_camera.camera = act->components.get<Camera>();
            main_camera.transform = act->components.get<core::transform>();
            break;
        }

        // ---- temp
        actor* new_actor = sg.create();
        core::transform* t = new_actor->components.create<core::transform>();
        t->scale = glm::vec3{ 0.1f, 0.1f, 0.1f };
        Material* material = new_actor->components.create<Material>();
        material->shader.load(std::string(RAIN_CONFIG->data_root + "/shaders/glsl/model.vert"), std::string(RAIN_CONFIG->data_root + "/shaders/glsl/model.frag"));
        core::mesh* mesh = new_actor->components.create<core::mesh>();

        Model model;
        model.path = file_path(RAIN_CONFIG->data_root + "/models/skelet/skeleton_animated.fbx");
        model.mesh = RAIN_FIND_DATA_FROM_PATH(Mesh, model.path.get_path_absolute());

        u32 vertices_count = model.mesh->data->vertices.size();
        glm::vec3* vertices = (glm::vec3*)malloc(vertices_count * sizeof(glm::vec3));
        for (u32 i = 0; i < vertices_count; ++i)
        {
            memcpy(&vertices[i], &(model.mesh->data->vertices[i].position), sizeof(glm::vec3));
        }

        {
            RAIN_PROFILE("quickhull");
            RAIN_LOG("vertices count : %u", vertices_count);
            quick_hull(vertices, vertices_count, mesh);
        }
        vao_quickhull = RAIN_RENDERER->load_primitive(mesh->vertices, mesh->vertices_count, mesh->vertices_indices, mesh->vertices_indices_count, mesh->normals, mesh->normals_count, mesh->normals_indices, mesh->normal_indices_count);

        // ---- /temp
    }

    void World::update_camera(const float _deltaTime)
    {
        update(*(main_camera.camera), *(main_camera.transform));
        RAIN_RENDERER->set_view_matrix(main_camera.transform->position, main_camera.transform->position + main_camera.camera->front, main_camera.camera->up);
    }

    void World::update_physics(const float _deltaTime)
    {
        std::vector<actor*> view;

        static float frequency = 1.0f;
        static float amplitude = 1.0f;
        static float lacunarity = 2.0f;
        static float persistence = 0.5f;
        bool change = false;
        if (RAIN_INPUT->is_key_released(DIK_U))
        {
            frequency += 0.2f;
            RAIN_LOG("frequency up : %f", frequency);
            change = true;
        }
        if (RAIN_INPUT->is_key_released(DIK_J))
        {
            frequency -= 0.2f;
            RAIN_LOG("frequency down : %f", frequency);
            change = true;
        }
        if (RAIN_INPUT->is_key_released(DIK_I))
        {
            amplitude += 0.2f;
            RAIN_LOG("amplitude up : %f", amplitude);
            change = true;
        }
        if (RAIN_INPUT->is_key_released(DIK_K))
        {
            amplitude -= 0.2f;
            RAIN_LOG("amplitude down : %f", amplitude);
            change = true;
        }
        if (RAIN_INPUT->is_key_released(DIK_O))
        {
            lacunarity += 0.2f;
            RAIN_LOG("lacunarity up : %f", lacunarity);
            change = true;
        }
        if (RAIN_INPUT->is_key_released(DIK_L))
        {
            lacunarity -= 0.2f;
            RAIN_LOG("lacunarity down : %f", lacunarity);
            change = true;
        }
        if (RAIN_INPUT->is_key_released(DIK_P))
        {
            persistence += 0.2f;
            RAIN_LOG("persistence up : %f", persistence);
            change = true;
        }
        if (RAIN_INPUT->is_key_released(DIK_M))
        {
            persistence -= 0.2f;
            RAIN_LOG("persistence down : %f", persistence);
            change = true;
        }

        voxel::update_map(&voxmap, main_camera.transform->position);

        sg.get_view<ui::text_field>(view);
        for (auto chat : view)
        {
            ui::update(*(chat->components.get<ui::text_field>()));
        }

        // applying springs
        //auto spring2_view = registry.view<Spring>();
        //for (auto entity : spring2_view)
        //{
        //    Spring& spring2 = spring2_view.get(entity);
        //    RigidBody& bodyA = registry.get<RigidBody>(spring2.entityA);
        //    core::transform& transformA = registry.get<core::transform>(spring2.entityA);
        //    RigidBody& bodyB = registry.get<RigidBody>(spring2.entityB);
        //    core::transform& transformB = registry.get<core::transform>(spring2.entityB);
        //    Physics::apply_spring(spring2, transformA, bodyA, transformB, bodyB);
        //}

        // updating physics
        sg.get_view<RigidBody, core::transform>(view, true);
        for (auto act : view)
        {
            RigidBody* body = act->components.get<RigidBody>();
            Physics::apply_gravity(*body);
            Physics::update(*body, *(act->components.get<core::transform>()), _deltaTime);
        }

        sg.get_view<RigidBody, core::sphere, core::transform>(view, true);
        for (auto act : view)
        {
            RigidBody* body1 = act->components.get<RigidBody>();
            core::sphere* bound1 = act->components.get<core::sphere>();
            core::transform* transform1 = act->components.get<core::transform>();

            sg.get_view<core::plane>(view, true);
            for (auto ent_plane : view)
            {
                core::plane* plane = ent_plane->components.get<core::plane>();
                HitInfo info = detect_collision_sphere_plane(*bound1, *transform1, *plane);
                if (info.hit)
                {
                    collision_response(*body1, *transform1, project_on_plane(transform1->position, *plane));
                }
            }

        }

    }

    void World::draw(const float _alpha)
    {
        //RAIN_WPROFILE("GPU : ", 1000.0f, 10.0f, 0.5f, (glm::vec4{ 0.5, 0.8f, 0.2f, 1.0f }));
        
        RAIN_RENDERER->update();

        voxel::draw_map(&voxmap, main_camera.transform->position);

        //std::vector<actor*> view;

        //sg.get_view<core::transform, Model, Material>(view);

        //for (auto entity : view)
        //{
        //    core::transform& t = *(entity->components.get<core::transform>());
        //    Model& model = *(entity->components.get<Model>());
        //    Material& material = *(entity->components.get<Material>());

        //    //Packet p {};
        //    //p.senderId = 47;
        //    //p.sequenceNumber= 12;
        //    //const char* hello = "qslmdfqkldfh";
        //    //memcpy(p.data, hello, strlen(hello));
        //    //
        //    //SerializedPacket* serialized = (SerializedPacket*)p.Serialize();

        //    //send_data(RAIN_APPLICATION.client, (char*)serialized, sizeof(SerializedPacket));
        //    //check_receive_data(RAIN_APPLICATION.client, buffer, sizeof(buffer));
        //    
        //    glm::vec3 position = t.position * _alpha + t.lastPosition * (1.0f - _alpha);
        //    glm::quat orientation = t.orientation * _alpha + t.lastOrientation * (1.0f - _alpha);

        //    RAIN_RENDERER->draw_mesh(model.mesh->data, material, position, orientation, t.scale);
        //}

        //sg.get_view<core::transform, core::mesh, Material>(view, true);
        //for (auto entity : view)
        //{
        //    core::transform& t = *(entity->components.get<core::transform>());
        //    core::mesh& mesh = *(entity->components.get<core::mesh>());
        //    Material& material = *(entity->components.get<Material>());

        //    glm::vec3 position = t.position * _alpha + t.lastPosition * (1.0f - _alpha);
        //    glm::quat orientation = t.orientation * _alpha + t.lastOrientation * (1.0f - _alpha);

        //    RAIN_RENDERER->draw_primitive(vao_quickhull, mesh.vertices_indices_count, material, t.position, t.orientation, t.scale);
        //}

        //sg.get_view<core::transform, core::sphere>(view, true);
        //for (auto entity : view)
        //{
        //    core::transform& t = *(entity->components.get<core::transform>());
        //    
        //    glm::vec3 position = t.position * _alpha + t.lastPosition * (1.0f - _alpha);
        //    glm::quat orientation = t.orientation * _alpha + t.lastOrientation * (1.0f - _alpha);

        //    RAIN_RENDERER->draw_sphere(position, orientation, t.scale);
        //}

        //sg.get_view<ui::text_field, ui::text_list>(view, true);
        //for (auto chat : view)
        //{
        //    ui::draw(*(chat->components.get<ui::text_field>()));
        //    ui::draw(*(chat->components.get<ui::text_list>()));
        //}
        ////auto plane_view = registry.view<Plane>();
        ////for (auto ent_plane : plane_view)
        ////{
        ////    Plane& plane = plane_view.get(ent_plane);
        ////    RAIN_RENDERER->draw_quad(plane, project_on_plane(vec3{0, 15, 0}, plane), vec30.7f, 0.7f, 0));
        ////}
    }
}