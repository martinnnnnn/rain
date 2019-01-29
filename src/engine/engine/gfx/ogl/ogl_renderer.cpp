#include "ogl_renderer.h"

//#include "stdio.h"
//#include <iostream>
#include <vector>

#include "core/core.h"
#include "ogl_shader.h"
#include "engine/core/context.h"
#include "engine/win32/win32_window.h"

#include <ft2build.h>
#include FT_FREETYPE_H


#include "math/geometry/algorithms/quick_hull.h"

namespace rain::engine
{
    using namespace rain::math;

    void Renderer::init()
    {
        GLenum res = glewInit();

        if (res != GLEW_OK)
        {
            RAIN_LOG("Error : %s\n", glewGetErrorString(res));
        }

        glEnable(GL_TEXTURE_2D);
        glShadeModel(GL_SMOOTH);
        glClearColor(0.1f, 0.1f, 0.1f, 0.0f);
        glClearDepth(1.0f);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
        glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
        resize(RAIN_WINDOW->size().x, RAIN_WINDOW->size().y);

        init_default_shaders();
        init_debug();
        init_shapes();
        init_text_2d();
    }

    void Renderer::resize(u32 _width, u32 _height)
    {
        glViewport(0, 0, _width, _height);
        set_perspective_projection_matrix(math::perspective_rh_no(to_rad(45.0f), f32(_width) / f32(_height), 0.1f, 10000.0f));
        set_orthogonal_projection_matrix(math::ortho(0.0f, f32(_width), 0.0f, f32(_height)));
        
    }

    void Renderer::init_default_shaders()
    {
        phong.load(RAIN_CONFIG->data_root + "/shaders/glsl/phong.vs", RAIN_CONFIG->data_root + "/shaders/glsl/phong.fs");

        phong.use();
        phong.set("lightDiff", 0.3f, 0.3f, 0.3f);
        phong.set("lightDirection", -0.2f, -1.0f, -0.3f);
    }

    void Renderer::set_perspective_projection_matrix(const mat4& _projection)
    {
        proj_mat_perspective = _projection;
    }

    void Renderer::set_orthogonal_projection_matrix(const mat4& _projection)
    {
        proj_mat_orthogonal = _projection;
        proj_mat_orthogonal = math::ortho(0.0f, static_cast<GLfloat>(800), 0.0f, static_cast<GLfloat>(600));
    }

    void Renderer::set_view_matrix(const vec3& _eye, float _pitch, float _yaw)
    {
        float cosPitch = cos(_pitch);
        float sinPitch = sin(_pitch);
        float cosYaw = cos(_yaw);
        float sinYaw = sin(_yaw);

        vec3 xaxis = { cosYaw, 0, -sinYaw };
        vec3 yaxis = { sinYaw * sinPitch, cosPitch, cosYaw * sinPitch };
        vec3 zaxis = { sinYaw * cosPitch, -sinPitch, cosPitch * cosYaw };

        view_mat =
        {
            vec4{ xaxis.x,              yaxis.x,                zaxis.x,                0 },
            vec4{ xaxis.y,              yaxis.y,                zaxis.y,                0 },
            vec4{ xaxis.z,              yaxis.z,                zaxis.z,                0 },
            vec4{ -dot(xaxis, _eye),    -dot(yaxis, _eye),      -dot(zaxis, _eye),      1 }
        };
    }

    void Renderer::set_view_matrix(const vec3& _eye, const vec3& _center, const vec3& _up)
    {
        view_mat = look_at_rh(_eye, _center, _up);
    }

    void Renderer::set_view_matrix(const mat4& _matrix)
    {
        view_mat = _matrix;
    }


    u32 Renderer::load_primitive(vec3* vertices, u32 vertices_count, u32* vertices_indices, u32 vertices_indices_count, vec3* normals, u32 normals_count, u32* normals_indices, u32 normals_indices_count)
    {
        u32 vao,
            vbo_vertices, ebo_vertices_indices,
            vbo_normals, ebo_normals_indices;

        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        glGenBuffers(1, &vbo_vertices);
        glGenBuffers(1, &ebo_vertices_indices);

        glBindBuffer(GL_ARRAY_BUFFER, vbo_vertices);
        glBufferData(GL_ARRAY_BUFFER, vertices_count * sizeof(vec3), vertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)nullptr);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_vertices_indices);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, vertices_indices_count * sizeof(u32), vertices_indices, GL_STATIC_DRAW);


        //glGenBuffers(1, &vbo_normals);
        //glGenBuffers(1, &ebo_normals_indices);

        //glBindBuffer(GL_ARRAY_BUFFER, vbo_normals);
        //glBufferData(GL_ARRAY_BUFFER, normals_count * sizeof(vec3), normals, GL_STATIC_DRAW);
        //glEnableVertexAttribArray(1);
        //glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)nullptr);

        //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_normals_indices);
        //glBufferData(GL_ELEMENT_ARRAY_BUFFER, normals_indices_count * sizeof(u32), normals_indices, GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glBindVertexArray(0);

        return vao;
    }

    void Renderer::draw_primitive(const u32 vao, const u32 nindices, const Material& material, const vec3& position, const quat& orientation, const vec3& scale)
    {
        //mat4 model = identity_mat4();
        //model = translate(model, position);

        material.shader.use();
        material.shader.set("model", translate(identity_mat4(), position) * mat4_cast(orientation) * math::scale(identity_mat4(), scale));
        material.shader.set("proj", proj_mat_perspective);
        material.shader.set("view", view_mat);

        glBindVertexArray(vao);
        glEnableVertexAttribArray(0);
        //glEnableVertexAttribArray(1);

        glDrawElements(GL_TRIANGLES, nindices, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

    void Renderer::load_mesh(Mesh * _mesh)
    {
        glGenVertexArrays(1, &_mesh->vao);
        glGenBuffers(1, &_mesh->vbo);
        glGenBuffers(1, &_mesh->ebo);

        glBindVertexArray(_mesh->vao);

        glBindBuffer(GL_ARRAY_BUFFER, _mesh->vbo);
        glBufferData(GL_ARRAY_BUFFER, _mesh->vertices.size() * sizeof(Vertex), &_mesh->vertices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _mesh->ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, _mesh->indices.size() * sizeof(unsigned int), &_mesh->indices[0], GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));

        glBindVertexArray(0);
    }

    void Renderer::release_mesh(Mesh* _mesh)
    {
        glDeleteBuffers(1, &_mesh->vbo);
        glDeleteBuffers(1, &_mesh->ebo);
        glDeleteVertexArrays(1, &_mesh->vao);

        _mesh->vbo = 0;
        _mesh->ebo = 0;
        _mesh->vao = 0;
    }

    void Renderer::draw_mesh(Mesh * _mesh, const Material& _material, const vec3& _position, const quat& _orientation, const vec3& _scale)
    {
        _material.shader.use();
        _material.shader.set("model", translate(identity_mat4(), _position) * mat4_cast(_orientation) * scale(identity_mat4(), _scale));
        _material.shader.set("proj", proj_mat_perspective);
        _material.shader.set("view", view_mat);

        glBindVertexArray(_mesh->vao);
        glDrawElements(GL_TRIANGLES, _mesh->indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        glActiveTexture(GL_TEXTURE0);
    }

    void Renderer::clear()
    {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void Renderer::draw_coord_view(const vec3& _position)
    {
        draw_debug_line(vec3{ 200.0f, 0.0f, 0.0f }, vec3{ -200.0f, 0.0f, 0.0f }, vec3{ 1.0f, 0.5f, 0.0f }, vec3{ 0.5f, 1.0f, 0.0f });
        draw_debug_line(vec3{ 0.0f, 200.0f, 0.0f }, vec3{ 0.0f, -200.0f, 0.0f }, vec3{ 0.0f, 1.0f, 0.5f }, vec3{ 0.0f, 0.5f, 1.0f });
        draw_debug_line(vec3{ 0.0f, 0.0f, 200.0f }, vec3{ 0.0f, 0.0f, -200.0f }, vec3{ 0.5f, 0.0f, 1.0f }, vec3{ 1.0f, 0.0f, 0.5f });
        //draw_debug_line(vec3{ -200.0f, 0.0f, 0.0f }, vec3{ 200.0f, 0.0f, 0.0f }, vec3{ 1.0f, 0.0f, 0.0f }, vec3{ 0.0f, 1.0f, 0.0f });
        //draw_debug_line(vec3{ 0.0f, 200.0f, 0.0f }, vec3{ 0.0f, -200.0f, 0.0f }, vec3{ 0.0f, 1.0f, 0.0f }, vec3{ 0.0f, 1.0f, 0.0f });
        //draw_debug_line(vec3{ 0.0f, 0.0f, 200.0f }, vec3{ 0.0f, 0.0f, -200.0f }, vec3{ 0.0f, 0.0f, 1.0f }, vec3{ 0.0f, 0.0f, 1.0f });
    }


    void Renderer::init_shapes()
    {
        init_cube();
        init_sphere();
    }

    void Renderer::init_cube()
    {
        const float vertices[288] = {
            // position           // normals          // texture coords
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
            0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
            0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
            0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
            0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
            0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
            0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
            -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

            0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
            0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
            0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
            0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
            0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
            0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
            0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
            0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
            0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
            0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
            0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
            0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
        };

        glGenVertexArrays(1, &cubeVAO);
        glBindVertexArray(cubeVAO);

        unsigned int cubeVBO;
        glGenBuffers(1, &cubeVBO);

        //unsigned int EBO;
        //glGenBuffers(1, &EBO);

        // binding

        glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        // position
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        // normals
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        // texture coord attribute
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);

        glBindVertexArray(0);
    }

    void Renderer::draw_quad(const Plane& _p, const vec3 _position, const vec3& _color)
    {   
        vec3 u = cross(_p.n, _position); // cross product -> note that u lies on the plane
        vec3 v = cross(_p.n, u); // v is orthogonal to both N and u (again is in the plane)  

        vec3 P0 = -_p.n * _p.D;        // "arbitrary" point
        float  f = 20.0f;  // large enough
        vec3 fu = u * f;
        vec3 fv = v * f;
        vec3 A = P0 - fu - fv;
        vec3 B = P0 + fu - fv;
        vec3 C = P0 + fu + fv;
        vec3 D = P0 - fu + fv;

        const float vertices[] =
        {
            A.x, A.y, A.z, _color.x, _color.y,_color.z,
            B.x, B.y, B.z, _color.x, _color.y,_color.z,
            C.x, C.y, C.z, _color.x, _color.y,_color.z,
            D.x, D.y, D.z, _color.x, _color.y,_color.z
        };

        unsigned int indices[] =
        {
            0, 1, 3,
            1, 2, 3
        };

        unsigned int VBO, EBO;
        glGenVertexArrays(1, &quadVAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(quadVAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ARRAY_BUFFER, 0);

        mat4 mvp = proj_mat_perspective * view_mat * identity_mat4();
        debug_shader.use();
        debug_shader.set("mvp", mvp);

        //phong.use();
        //phong.set("model", translate(mat4(1), vec3(0,0,0)));
        //phong.set("proj", proj_mat_perspective);
        //phong.set("view", view_mat);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

    void Renderer::init_sphere()
    {
        glGenVertexArrays(1, &sphereVAO);

        unsigned int vbo, ebo;
        glGenBuffers(1, &vbo);
        glGenBuffers(1, &ebo);

        std::vector<vec3> positions;
        std::vector<vec2> uv;
        std::vector<vec3> normals;
        std::vector<unsigned int> indices;

        const unsigned int X_SEGMENTS = 64;
        const unsigned int Y_SEGMENTS = 64;
        const float PI = 3.14159265359f;
        for (unsigned int y = 0; y <= Y_SEGMENTS; ++y)
        {
            for (unsigned int x = 0; x <= X_SEGMENTS; ++x)
            {
                float xSegment = (float)x / (float)X_SEGMENTS;
                float ySegment = (float)y / (float)Y_SEGMENTS;
                float xPos = math::cos(xSegment * 2.0f * PI) * math::sin(ySegment * PI);
                float yPos = math::cos(ySegment * PI);
                float zPos = math::sin(xSegment * 2.0f * PI) * math::sin(ySegment * PI);

                positions.push_back(vec3{ xPos, yPos, zPos });
                uv.push_back(vec2{ xSegment, ySegment });
                normals.push_back(vec3{ xPos, yPos, zPos });
            }
        }

        bool oddRow = false;
        for (int y = 0; y < Y_SEGMENTS; ++y)
        {
            if (!oddRow) // even rows: y == 0, y == 2; and so on
            {
                for (int x = 0; x <= X_SEGMENTS; ++x)
                {
                    indices.push_back(y       * (X_SEGMENTS + 1) + x);
                    indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
                }
            }
            else
            {
                for (int x = X_SEGMENTS; x >= 0; --x)
                {
                    indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
                    indices.push_back(y       * (X_SEGMENTS + 1) + x);
                }
            }
            oddRow = !oddRow;
        }
        sphere_index_count = indices.size();

        std::vector<float> data;
        for (u32 i = 0; i < positions.size(); ++i)
        {
            data.push_back(positions[i].x);
            data.push_back(positions[i].y);
            data.push_back(positions[i].z);
            if (uv.size() > 0)
            {
                data.push_back(uv[i].x);
                data.push_back(uv[i].y);
            }
            if (normals.size() > 0)
            {
                data.push_back(normals[i].x);
                data.push_back(normals[i].y);
                data.push_back(normals[i].z);
            }
        }
        glBindVertexArray(sphereVAO);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), &data[0], GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
        i32 stride = (3 + 2 + 3) * sizeof(float);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*)(5 * sizeof(float)));
    }


    void Renderer::init_debug()
    {
        glGenVertexArrays(1, &m_debug_vao);
        glBindVertexArray(m_debug_vao);

        glGenBuffers(1, &m_debug_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, m_debug_vbo);
        glBufferData(GL_ARRAY_BUFFER, debug_vertices_max_count * sizeof(vec3), &m_debug_vertices[0], GL_DYNAMIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)nullptr);

        glGenBuffers(1, &m_debug_cbo);
        glBindBuffer(GL_ARRAY_BUFFER, m_debug_cbo);
        glBufferData(GL_ARRAY_BUFFER, debug_vertices_max_count * sizeof(vec3), &m_debug_colors[0], GL_DYNAMIC_DRAW);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)nullptr);

        glEnableVertexAttribArray(0);
        glBindVertexArray(0);

        debug_shader.load(RAIN_CONFIG->data_root + "/shaders/glsl/debug.vs", RAIN_CONFIG->data_root + "/shaders/glsl/debug.fs");
    }

    void Renderer::draw()
    {
        draw_coord_view(vec3 {});
        draw_debug();
    }

    void Renderer::draw_debug()
    {
        if (m_debug_vertex_count == 0)
        {
            return;
        }

        glBindBuffer(GL_ARRAY_BUFFER, m_debug_vbo);
        glBufferSubData(GL_ARRAY_BUFFER, 0, m_debug_vertex_count * sizeof(vec3), m_debug_vertices);

        glBindBuffer(GL_ARRAY_BUFFER, m_debug_cbo);
        glBufferSubData(GL_ARRAY_BUFFER, 0, m_debug_vertex_count * sizeof(vec3), m_debug_colors);

        glDisable(GL_BLEND);

        mat4 mvp = proj_mat_perspective * view_mat * identity_mat4();
        debug_shader.use();
        debug_shader.set("mvp", mvp);

        glBindVertexArray(m_debug_vao);
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);

        glDrawArrays(GL_LINES, 0, m_debug_vertex_count);

        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(0);
        glBindVertexArray(0);

        glEnable(GL_BLEND);

        m_debug_vertex_count = 0;
    }

    void Renderer::draw_debug_line(const vec3& _point1, const vec3& _point2, const vec3& _color)
    {
        if (m_debug_vertex_count + 2 < debug_vertices_max_count)
        {
            m_debug_vertices[m_debug_vertex_count + 0] = _point1;
            m_debug_vertices[m_debug_vertex_count + 1] = _point2;
            m_debug_colors[m_debug_vertex_count + 0] = _color;
            m_debug_colors[m_debug_vertex_count + 1] = _color;

            m_debug_vertex_count += 2;
        }
    }

    void Renderer::draw_debug_line(const vec3& _point1, const vec3& _point2, const vec3& _color1, const vec3& _color2)
    {
        if (m_debug_vertex_count + 2 < debug_vertices_max_count)
        {
            m_debug_vertices[m_debug_vertex_count + 0] = _point1;
            m_debug_vertices[m_debug_vertex_count + 1] = _point2;
            m_debug_colors[m_debug_vertex_count + 0] = _color1;
            m_debug_colors[m_debug_vertex_count + 1] = _color2;

            m_debug_vertex_count += 2;
        }
    }

    void Renderer::draw_debug_cube(const vec3& _center, const f32 _width, const f32 _height, const vec3& _color)
    {
        float halfWidth = _width / 2.0f;
        float halfHeight = _height / 2.0f;

        vec3 A{ _center.x - halfWidth, _center.y + halfWidth, _center.z - halfHeight };
        vec3 B{ _center.x + halfWidth, _center.y + halfWidth, _center.z - halfHeight };
        vec3 C{ _center.x + halfWidth, _center.y - halfWidth, _center.z - halfHeight };
        vec3 D{ _center.x - halfWidth, _center.y - halfWidth, _center.z - halfHeight };
        vec3 E{ _center.x - halfWidth, _center.y + halfWidth, _center.z + halfHeight };
        vec3 F{ _center.x + halfWidth, _center.y + halfWidth, _center.z + halfHeight };
        vec3 G{ _center.x + halfWidth, _center.y - halfWidth, _center.z + halfHeight };
        vec3 H{ _center.x - halfWidth, _center.y - halfWidth, _center.z + halfHeight };

        draw_debug_line(A, B, _color);
        draw_debug_line(B, C, _color);
        draw_debug_line(C, D, _color);
        draw_debug_line(D, A, _color);

        draw_debug_line(A, E, _color);
        draw_debug_line(B, F, _color);
        draw_debug_line(C, G, _color);
        draw_debug_line(D, H, _color);

        draw_debug_line(E, F, _color);
        draw_debug_line(F, G, _color);
        draw_debug_line(G, H, _color);
        draw_debug_line(H, E, _color);
    }

    void Renderer::draw_debug_sphere(const vec3& _position, const f32 _scale, const quat& orientation)
    {
        assert(false);
    }

    void Renderer::draw_billboard()
    {
        assert(false);
    }

    void Renderer::draw_line()
    {
        assert(false);
    }

    //void Renderer::draw_quad(const core::math::Plane& _p, const vec3 _position, const vec3& _color)
    //{
    //    vec3 z(0.0f, 0.0f, 1.0f);
    //    vec3 u = cross(_p.n, _position); // cross product -> note that u lies on the plane
    //    vec3 v = cross(_p.n, u); // v is orthogonal to both N and u (again is in the plane)  

    //    vec3 P0 = -_p.n * _p.D;        // "arbitrary" point
    //    float  f = 200.0f;  // large enough
    //    vec3 fu = u * f;
    //    vec3 fv = v * f;
    //    vec3 A = P0 - fu - fv;
    //    vec3 B = P0 + fu - fv;
    //    vec3 C = P0 + fu + fv;
    //    vec3 D = P0 - fu + fv;

    //    draw_debug_line(A, B, _color);
    //    draw_debug_line(B, C, _color);
    //    draw_debug_line(C, D, _color);
    //    draw_debug_line(D, A, _color);
    //}

    void Renderer::draw_cube(const vec3& _position, const f32 _scale,  const quat& _orientation)
    {
        phong.use();
        phong.set("model", translate(identity_mat4(), _position) * mat4_cast(_orientation) * scale(identity_mat4(), vec3{ _scale,_scale,_scale }));
        phong.set("proj", proj_mat_perspective);
        phong.set("view", view_mat);

        glBindVertexArray(cubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
    }

    void Renderer::draw_sphere(const vec3& _center, const f32 _scale, const quat& orientation)
    {
        phong.use();
        phong.set("model", translate(identity_mat4(), _center) * mat4_cast(orientation) * scale(identity_mat4(), vec3{ _scale,_scale,_scale }));
        phong.set("proj", proj_mat_perspective);
        phong.set("view", view_mat);

        glBindVertexArray(sphereVAO);
        glDrawElements(GL_TRIANGLE_STRIP, sphere_index_count, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        //glBindVertexArray(quadVAO);
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        //glBindVertexArray(0);
    }

    void Renderer::draw_mesh()
    {
        assert(false);
    }

    void Renderer::init_text_2d()
    {
        text_shader.load(RAIN_CONFIG->data_root + "/shaders/glsl/text_2d.vs", RAIN_CONFIG->data_root + "/shaders/glsl/text_2d.fs");

        text_shader.use();
        text_shader.set("projection", proj_mat_orthogonal);

        FT_Library ft;

        if (FT_Init_FreeType(&ft))
        {
            RAIN_LOG_ERROR("ERROR::FREETYPE: Could not init FreeType Library");
            return;
        }

        std::string path_to_font = RAIN_CONFIG->data_root + "/fonts/arial.ttf";
        FT_Face face;
        if (FT_New_Face(ft, path_to_font.c_str(), 0, &face))
        {
            RAIN_LOG_ERROR("ERROR::FREETYPE: Failed to load font");
            return;
        }

        FT_Set_Pixel_Sizes(face, 0, 48);

        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        for (GLubyte c = 0; c < 128; c++)
        {
            if (FT_Load_Char(face, c, FT_LOAD_RENDER))
            {
                RAIN_LOG_ERROR("ERROR::FREETYTPE: Failed to load Glyph");
                continue;
            }
            GLuint texture;
            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);
            glTexImage2D(
                GL_TEXTURE_2D,
                0,
                GL_RED,
                face->glyph->bitmap.width,
                face->glyph->bitmap.rows,
                0,
                GL_RED,
                GL_UNSIGNED_BYTE,
                face->glyph->bitmap.buffer
            );

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            RChar character =
            {
                texture,
                {face->glyph->bitmap.width, face->glyph->bitmap.rows},
                {face->glyph->bitmap_left, face->glyph->bitmap_top},
                GLuint(face->glyph->advance.x)
            };
            text_characters.insert(std::pair<GLchar, RChar>(c, character));
        }
        glBindTexture(GL_TEXTURE_2D, 0);

        FT_Done_Face(face);
        FT_Done_FreeType(ft);

        glGenVertexArrays(1, &text_vao);
        glGenBuffers(1, &text_vbo);
        glBindVertexArray(text_vao);
        glBindBuffer(GL_ARRAY_BUFFER, text_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    void Renderer::draw_text_2d(const std::string& _text, const f32 _x, const f32 _y, const f32 _scale, const vec3& _color)
    {
        glEnable(GL_CULL_FACE);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        text_shader.use();
        text_shader.set("textColor", _color);

        glActiveTexture(GL_TEXTURE0);
        glBindVertexArray(text_vao);

        std::string::const_iterator c;
        GLfloat x = _x;
        for (c = _text.begin(); c != _text.end(); c++)
        {
            RChar ch = text_characters[*c];

            GLfloat xpos = x + ch.bearing[0] * _scale;
            GLfloat ypos = _y - (ch.size[1] - ch.bearing[1]) * _scale;

            GLfloat w = ch.size[0] * _scale;
            GLfloat h = ch.size[1] * _scale;

            GLfloat vertices[6][4] =
            {
                { xpos,     ypos + h,   0.0, 0.0 },
                { xpos,     ypos,       0.0, 1.0 },
                { xpos + w, ypos,       1.0, 1.0 },

                { xpos,     ypos + h,   0.0, 0.0 },
                { xpos + w, ypos,       1.0, 1.0 },
                { xpos + w, ypos + h,   1.0, 0.0 }
            };

            glBindTexture(GL_TEXTURE_2D, ch.texture_id);

            glBindBuffer(GL_ARRAY_BUFFER, text_vbo);
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

            glBindBuffer(GL_ARRAY_BUFFER, 0);

            glDrawArrays(GL_TRIANGLES, 0, 6);

            x += (ch.advance >> 6) * _scale;
        }
        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}