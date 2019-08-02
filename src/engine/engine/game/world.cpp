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


namespace rain::engine
{
    void World::init(const std::string& _path)
    {
        RAIN_PROFILE("world init");

        {
            RAIN_PROFILE("Chunk init");
            vmap = new core::voxel_map();
            core::init(vmap);
        }


        file.open(_path);
        json_reader::read_world(file.read(), *this);

        auto chat_view = registry.view<ui::text_field, ui::text_list>();
        for (auto chat : chat_view)
        {
            ui::text_field& field = chat_view.get<ui::text_field>(chat);
            ui::text_list& list = chat_view.get<ui::text_list>(chat);
            field.on_validate.connect_member(RAIN_CONTEXT->app, &application::send_to_network);
            RAIN_MESSAGING->subscribe<network::INGAME_CHAT_INC>(std::bind(&ui::text_list::add_line, &list, std::placeholders::_1));
            break;
        }

        // ---- temp
        auto entity = registry.create();
        core::transform& t = registry.assign<core::transform>(entity);
        t.scale = glm::vec3{ 0.1f, 0.1f, 0.1f };
        Material& material = registry.assign<Material>(entity);
        material.shader.load(std::string(RAIN_CONFIG->data_root + "/shaders/glsl/model.vs"), std::string(RAIN_CONFIG->data_root + "/shaders/glsl/model.fs"));
        core::mesh& mesh = registry.assign<core::mesh>(entity);

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
            quick_hull(vertices, vertices_count, &mesh);
        }
        temp_vao = RAIN_RENDERER->load_primitive(mesh.vertices, mesh.vertices_count, mesh.vertices_indices, mesh.vertices_indices_count, mesh.normals, mesh.normals_count, mesh.normals_indices, mesh.normal_indices_count);
        // ---- /temp
    }

    void World::update_camera(const float _deltaTime)
    {
        auto camera_view = registry.view<core::transform, Camera>();
        for (auto entity : camera_view)
        {
            core::transform& t = camera_view.get<core::transform>(entity);
            Camera& camera = camera_view.get<Camera>(entity);
            update(camera, t);
            RAIN_RENDERER->set_view_matrix(t.position, t.position + camera.front, camera.up);
        }
    }

    void World::update_physics(const float _deltaTime)
    {
        auto chat_view = registry.view<ui::text_field>();
        for (auto chat : chat_view)
        {
            ui::update(chat_view.get(chat));
        }

        // applying springs
        auto spring2_view = registry.view<Spring>();
        for (auto entity : spring2_view)
        {
            Spring& spring2 = spring2_view.get(entity);
            RigidBody& bodyA = registry.get<RigidBody>(spring2.entityA);
            core::transform& transformA = registry.get<core::transform>(spring2.entityA);
            RigidBody& bodyB = registry.get<RigidBody>(spring2.entityB);
            core::transform& transformB = registry.get<core::transform>(spring2.entityB);
            Physics::apply_spring(spring2, transformA, bodyA, transformB, bodyB);
        }

        // updating physics
        auto physics_view = registry.view<RigidBody, core::transform>();
        for (auto entity : physics_view)
        {
            RigidBody& body = physics_view.get<RigidBody>(entity);
            Physics::apply_gravity(body);
            Physics::update(body, physics_view.get<core::transform>(entity), _deltaTime);
        }

        //auto transform_view = registry.view<Transform>();
        //for (auto entity1 : transform_view)
        //{
        //    Transform& transform1 = transform_view.get(entity1);
        //    for (auto entity2 : transform_view)
        //    {
        //        if (entity1 == entity2)
        //            continue;

        //        Transform& transform2 = transform_view.get(entity2);
        //        
        //        detect_collision_gjk(positions, transform1, positions, transform2);
        //    }
        //}
        // updating collision
        auto sphere_view = registry.view<RigidBody, core::sphere, core::transform>();
        for (auto entity1 : sphere_view)
        {
            RigidBody& body1 = sphere_view.get<RigidBody>(entity1);
            core::sphere& bound1 = sphere_view.get<core::sphere>(entity1);
            core::transform& transform1 = sphere_view.get<core::transform>(entity1);
            //for (auto entity2 : sphere_view)
            //{
            //    if (entity1 == entity2)
            //    {
            //        break;
            //    }

            //    RigidBody& body2 = sphere_view.get<RigidBody>(entity2);
            //    Sphere& bound2 = sphere_view.get<Sphere>(entity2);
            //    Transform& transform2 = sphere_view.get<Transform>(entity2);

            //    HitInfo info = detect_collision_sphere(bound1, transform1, bound2, transform2);
            //    if (info.hit)
            //    {
            //        collision_response(body1, transform1, body2, transform2);
            //    }
            //}

            auto plane_view = registry.view<core::plane>();
            for (auto ent_plane : plane_view)
            {
                core::plane& plane = plane_view.get(ent_plane);

                HitInfo info = detect_collision_sphere_plane(bound1, transform1, plane);
                if (info.hit)
                {
                    collision_response(body1, transform1, project_on_plane(transform1.position, plane));
                }
            }

        }

    }

    void draw_naive(const core::voxel_chunk& chunk);

    void World::draw(const float _alpha)
    {
        RAIN_WPROFILE("world render ", 500.0f, 50.0f, 0.2f, (glm::vec4{ 0.5, 0.8f, 0.2f, 1.0f }));

        {
            RAIN_WPROFILE("chunk render ", 500.0f, 55.0f, 0.2f, (glm::vec4{ 0.2, 0.9f, 0.2f, 1.0f }));
            for (u32 i = 0; i < vmap->chunks_count; ++i)
            {
                draw_naive(vmap->chunks[i]);
            }
        }


        auto view = registry.view<core::transform, Model, Material>();

        for (auto entity : view)
        {
            core::transform& t = view.get<core::transform>(entity);
            Model& model = view.get<Model>(entity);
            Material& material = view.get<Material>(entity);

            //Packet p {};
            //p.senderId = 47;
            //p.sequenceNumber= 12;
            //const char* hello = "qslmdfqkldfh";
            //memcpy(p.data, hello, strlen(hello));
            //
            //SerializedPacket* serialized = (SerializedPacket*)p.Serialize();

            //send_data(RAIN_APPLICATION.client, (char*)serialized, sizeof(SerializedPacket));
            //check_receive_data(RAIN_APPLICATION.client, buffer, sizeof(buffer));
            
            glm::vec3 position = t.position * _alpha + t.lastPosition * (1.0f - _alpha);
            glm::quat orientation = t.orientation * _alpha + t.lastOrientation * (1.0f - _alpha);

            RAIN_RENDERER->draw_mesh(model.mesh->data, material, position, orientation, t.scale);
        }

        auto test_view = registry.view<core::transform, core::mesh, Material>();
        for (auto entity : test_view)
        {
            core::transform& t = test_view.get<core::transform>(entity);
            core::mesh& mesh = test_view.get<core::mesh>(entity);
            Material& material = test_view.get<Material>(entity);

            glm::vec3 position = t.position * _alpha + t.lastPosition * (1.0f - _alpha);
            glm::quat orientation = t.orientation * _alpha + t.lastOrientation * (1.0f - _alpha);

            RAIN_RENDERER->draw_primitive(temp_vao, mesh.vertices_indices_count, material, t.position, t.orientation, t.scale);
        }

        auto view2 = registry.view<core::transform, core::sphere>();

        for (auto entity : view2)
        {
            core::transform& t = view2.get<core::transform>(entity);
            
            glm::vec3 position = t.position * _alpha + t.lastPosition * (1.0f - _alpha);
            glm::quat orientation = t.orientation * _alpha + t.lastOrientation * (1.0f - _alpha);

            RAIN_RENDERER->draw_sphere(position, orientation, t.scale);
        }

        auto chat_view = registry.view<ui::text_field, ui::text_list>();
        for (auto chat : chat_view)
        {
            ui::draw(chat_view.get<ui::text_field>(chat));
            ui::draw(chat_view.get<ui::text_list>(chat));
        }
        //auto plane_view = registry.view<Plane>();
        //for (auto ent_plane : plane_view)
        //{
        //    Plane& plane = plane_view.get(ent_plane);
        //    RAIN_RENDERER->draw_quad(plane, project_on_plane(vec3{0, 15, 0}, plane), vec30.7f, 0.7f, 0));
        //}
    }

    void draw_naive(const core::voxel_chunk& chunk)
    {
        using core::CHUNK_SIZE;
        using core::CHUNK_SIZE_SQUARED;
        using core::voxel_block;

        for (u32 i = 0; i < CHUNK_SIZE; ++i)
        {
            for (u32 j = 0; j < CHUNK_SIZE; ++j)
            {
                for (u32 k = 0; k < CHUNK_SIZE; ++k)
                {
                    const i32 index = i + j * CHUNK_SIZE + k * CHUNK_SIZE_SQUARED;
                    const voxel_block* block = &(chunk.data[index]);

                    if (block->type == voxel_block::Type::EMPTY)
                    {
                        continue;
                    }

                    u32 iabsolute = i + chunk.position.x;
                    u32 jabsolute = j + chunk.position.y;
                    u32 kabsolute = k + chunk.position.z;

                    //if (iabsolute == 0 || jabsolute == 0 || kabsolute == 0 || iabsolute == CHUNK_SIZE - 1 || jabsolute == CHUNK_SIZE - 1 || kabsolute == CHUNK_SIZE - 1)
                    //{
                    //    RAIN_RENDERER->draw_sphere(glm::vec3{ iabsolute + 15, jabsolute, kabsolute }, glm::quat(), glm::vec3{ 0.3f, 0.3f, 0.3f });
                    //    continue;
                    //}

                    if (!is_block_border(&chunk, core::uvec3{ i, j, k }))
                    {
                        continue;
                    }

                    RAIN_RENDERER->draw_sphere(glm::vec3{ iabsolute + 15, jabsolute, kabsolute }, glm::quat(), glm::vec3{ 0.3f, 0.3f, 0.3f });
                }
            }
        }
    }

    //void generate_mesh(Chunk& chunk)
    //{
    //    chunk.cXN = chunk.position.x > 0 ? &chunk.map->chunks[chunk.position.x - 1, chunk.position.y, chunk.position.z] : nullptr;
    //    chunk.cYN = chunk.position.y > 0 ? &chunk.map->chunks[chunk.position.x, chunk.position.y - 1, chunk.position.z] : nullptr;
    //    chunk.cZN = chunk.position.z > 0 ? &chunk.map->chunks[chunk.position.x, chunk.position.y, chunk.position.z - 1] : nullptr;

    //    chunk.cXP = chunk.position.x < CHUNK_MAX_POS_X - 1 ? &chunk.map->chunks[chunk.position.x + 1, chunk.position.y, chunk.position.z] : nullptr;
    //    chunk.cYP = chunk.position.y < CHUNK_MAX_POS_Y - 1 ? &chunk.map->chunks[chunk.position.x, chunk.position.y + 1, chunk.position.z] : nullptr;
    //    chunk.cZP = chunk.position.z < CHUNK_MAX_POS_Z - 1 ? &chunk.map->chunks[chunk.position.x, chunk.position.y, chunk.position.z + 1] : nullptr;


    //    u32 access = 0;

    //    // Y axis - start from the bottom and search up
    //    for (u32 j = 0; j < CHUNK_SIZE; ++j)
    //    {
    //        // Z axis
    //        for (u32 k = 0; k < CHUNK_SIZE; ++k)
    //        {
    //            // X axis
    //            for (u32 i = 0; i < CHUNK_SIZE; ++i)
    //            {
    //                access = i + j * CHUNK_SIZE + k * CHUNK_SIZE_SQUARED;
    //                Block* b = &(chunk.data[access]);

    //                if (b->type == BlockType::EMPTY)
    //                {
    //                    continue;
    //                }

    //                create_run(chunk, b, i, j, k, access);
    //            }

    //            //if (vertexBuffer.used > vertexBuffer.data.Length - 2048)
    //            //    vertexBuffer.Extend(2048);
    //        }
    //    }
    //}

    //void create_run(Chunk& chunk, Block* b, u32 i, u32 j, u32 k, u32 access)
    //{
    //    static bool visited[CHUNK_SIZE_CUBED * CHUNK_NEIGHBOUR_COUNT];
    //    memset(&visited, 0, sizeof(bool) * CHUNK_SIZE_CUBED * CHUNK_NEIGHBOUR_COUNT);

    //    int i1 = i + 1;
    //    int j1 = j + 1;
    //    int k1 = k + 1;

    //    int length = 0;
    //    int chunkAccess = 0;

    //    //if (!visited[CHUNK_CXN * CHUNK_SIZE_CUBED + access] && visible_face_XN(i - 1, j, k))
    //    //{
    //    //    // Search upwards to determine run length
    //    //    for (int q = j; q < CHUNK_SIZE; q++)
    //    //    {
    //    //        // Pre-calculate the array lookup as it is used twice
    //    //        chunkAccess = i + q * CHUNK_SIZE + k * CHUNK_SIZE_SQUARED;

    //    //        // If we reach a different block or an empty block, end the run
    //    //        if (DifferentBlock(chunkAccess, b))
    //    //            break;

    //    //        // Store that we have visited this block
    //    //        chunkHelper.visitedXN[chunkAccess] = true;

    //    //        length++;
    //    //    }


    //    //    if (length > 0)
    //    //    {
    //    //        // Create a quad and write it directly to the buffer
    //    //        BlockVertex.AppendQuad(buffer, new Int3(i, length + j, k1),
    //    //            new Int3(i, length + j, k),
    //    //            new Int3(i, j, k1),
    //    //            new Int3(i, j, k),
    //    //            (byte)FaceType.xn, b.kind, health16);

    //    //        buffer.used += 6;
    //    //    }
    //    //}

    //    //// Same algorithm for right (X+)
    //    //if (!chunkHelper.visitedXP[access] && VisibleFaceXP(i1, j, k))
    //    //{
    //    //    ...
    //    //}
    //}

    //bool visible_face_XN(Chunk& chunk, u32 i, u32 j, u32 k)
    //{
    //    // Access directly from a neighbouring chunk
    //    if (i < 0)
    //    {
    //        Chunk* cXN = chunk.cXN;
    //        if (cXN == nullptr)
    //        {
    //            return true;
    //        }

    //        return chunk.cXN->data[31 + j * CHUNK_SIZE + k * CHUNK_SIZE_SQUARED].type == BlockType::EMPTY;
    //    }

    //    return chunk.data[i + j * CHUNK_SIZE + k * CHUNK_SIZE_SQUARED].type == BlockType::EMPTY;
    //}

    //bool visible_face_XP(Chunk& chunk, u32 i, u32 j, u32 k)
    //{
    //    if (i >= CHUNK_SIZE)
    //    {
    //        Chunk* cXP = chunk.cXP;
    //        if (cXP == nullptr)
    //            return true;

    //        return cXP->data[0 + j * CHUNK_SIZE + k * CHUNK_SIZE_SQUARED].type == BlockType::EMPTY;
    //    }

    //    return chunk.data[i + j * CHUNK_SIZE + k * CHUNK_SIZE_SQUARED].type == BlockType::EMPTY;
    //}

}