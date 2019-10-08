#include "ogl_renderer.h"

//#include "stdio.h"
//#include <iostream>
#include <vector>

#include "core/core.h"
#include "ogl_shader.h"
#include "engine/core/context.h"
#include "engine/core/profiler.h"
#include "engine/win32/win32_window.h"
#include "engine/data/data_system.h"
#include "engine/win32/win32_input.h"

#include <ft2build.h>
#include FT_FREETYPE_H

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

//#include "math/geometry/algorithms/quick_hull.h"

namespace rain::engine
{
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
        glDisable(GL_CULL_FACE);
        glDepthFunc(GL_LEQUAL);
        glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
        resize(u32(RAIN_WINDOW->size().x), u32(RAIN_WINDOW->size().y));

#ifdef SPIRV
        gl_specialize_shader = (PFNGLSPECIALIZESHADERPROC)glGetProcAddressREGAL("glSpecializeShaderARB");
        if (!gl_specialize_shader) 
        {
            fprintf(stderr, "failed to load glSpecializeShaderARB entry point\n");
        }
#endif
    }

    void Renderer::init_data()
    {
        RAIN_PROFILE("Renderer data initialization");
        init_default_shaders();
        init_debug();
        init_ui();
        init_shapes();
        init_text_2d();
    }

    void Renderer::update()
    {
        if (RAIN_INPUT->is_key_released(DIK_R) && RAIN_INPUT->is_key_pressed(DIK_LCONTROL))
        {
            RAIN_DATA_SYSTEM->reload_shaders();
            init_default_shaders();
        }
    }

    void Renderer::resize(u32 _width, u32 _height)
    {
        glViewport(0, 0, _width, _height);
        set_perspective_projection_matrix(glm::perspective(glm::radians(45.0f), f32(_width) / f32(_height), 0.1f, 10000.0f));
        set_orthogonal_projection_matrix(glm::ortho(0.0f, f32(_width), 0.0f, f32(_height)));
        
    }

    void Renderer::init_default_shaders()
    {
        instancing_handle = RAIN_FIND_DATA_FROM_PATH(Shader, RAIN_CONFIG->data_root + "/shaders/glsl/instancing.vert");
        instancing_handle->data->use();
        instancing_handle->data->set("dirLight.direction", -0.2f, -1.0f, -0.3f);
        instancing_handle->data->set("dirLight.ambient", glm::vec3(1.0f, 1.0f, 1.0f));
        instancing_handle->data->set("dirLight.diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
        instancing_handle->data->set("dirLight.specular", glm::vec3(1.0f, 1.0f, 1.0f));
        instancing_handle->data->set("material.shininess", 32.0f);

        sf_handle = RAIN_FIND_DATA_FROM_PATH(Shader, RAIN_CONFIG->data_root + "/shaders/glsl/scalar_field.vert");
        sf_handle->data->use();
        sf_handle->data->set("dirLight.direction", -0.2f, -1.0f, -0.3f);
        sf_handle->data->set("dirLight.ambient", glm::vec3(1.0f, 1.0f, 1.0f));
        sf_handle->data->set("dirLight.diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
        sf_handle->data->set("dirLight.specular", glm::vec3(1.0f, 1.0f, 1.0f));
        sf_handle->data->set("material.shininess", 32.0f);

        phong_handle = RAIN_FIND_DATA_FROM_PATH(Shader, RAIN_CONFIG->data_root + "/shaders/glsl/phong.vert");
        phong_handle->data->use();
        phong_handle->data->set("lightDiff", 0.3f, 0.3f, 0.3f); 
        phong_handle->data->set("lightDirection", -0.2f, -1.0f, -0.3f);

        transvoxel_handle = RAIN_FIND_DATA_FROM_PATH(Shader, RAIN_CONFIG->data_root + "/shaders/glsl/transvoxel.vert");
        transvoxel_handle->data->use();
        transvoxel_handle->data->set("lightPos", 0.3f, 0.3f, 0.3f);
        transvoxel_handle->data->set("viewPos", 0.3f, 0.3f, 0.3f);
        transvoxel_handle->data->set("lightColor", 1.0f, 1.0f, 1.0f);
        transvoxel_handle->data->set("objectColor", 1.0f, 0.5f, 0.31f);
    }

    void Renderer::set_perspective_projection_matrix(const glm::mat4& _projection)
    {
        proj_mat_perspective = _projection;
    }

    void Renderer::set_orthogonal_projection_matrix(const glm::mat4& mat)
    {
        proj_mat_orthogonal = mat;
        //proj_mat_orthogonal = glm::ortho(0.0f, static_cast<GLfloat>(800), 0.0f, static_cast<GLfloat>(600));
    }

    void Renderer::set_view_matrix(const glm::vec3& _eye, float _pitch, float _yaw)
    {
        float cosPitch = cos(_pitch);
        float sinPitch = sin(_pitch);
        float cosYaw = cos(_yaw);
        float sinYaw = sin(_yaw);

        glm::vec3 xaxis = { cosYaw, 0, -sinYaw };
        glm::vec3 yaxis = { sinYaw * sinPitch, cosPitch, cosYaw * sinPitch };
        glm::vec3 zaxis = { sinYaw * cosPitch, -sinPitch, cosPitch * cosYaw };

        view_mat =
        {
            glm::vec4{ xaxis.x,              yaxis.x,                zaxis.x,                0 },
            glm::vec4{ xaxis.y,              yaxis.y,                zaxis.y,                0 },
            glm::vec4{ xaxis.z,              yaxis.z,                zaxis.z,                0 },
            glm::vec4{ -dot(xaxis, _eye),    -dot(yaxis, _eye),      -dot(zaxis, _eye),      1 }
        };
    }

    void Renderer::set_view_matrix(const glm::vec3& _eye, const glm::vec3& _center, const glm::vec3& _up)
    {
        view_mat = glm::lookAt(_eye, _center, _up);
    }

    void Renderer::set_view_matrix(const glm::mat4& _matrix)
    {
        view_mat = _matrix;
    }

    u32 Renderer::load_scalar_field(core::scalar_field_mesh* mesh)
    {
        std::vector<float> data;
        for (u32 i = 0; i < mesh->vertices.size(); ++i)
        {
            data.push_back(mesh->vertices[i].x);
            data.push_back(mesh->vertices[i].y);
            data.push_back(mesh->vertices[i].z);
            if (mesh->uv.size() > 0)
            {
                data.push_back(mesh->uv[i].x);
                data.push_back(mesh->uv[i].y);
            }                  
            if (mesh->normals.size() > 0)
            {
                data.push_back(mesh->normals[i].x);
                data.push_back(mesh->normals[i].y);
                data.push_back(mesh->normals[i].z);
            }
        }

        u32 vao, vbo_vertices, ebo_vertices_indices;

        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);
        glGenBuffers(1, &vbo_vertices);

        i32 stride = (3 + 2 + 3) * sizeof(float);

        glBindBuffer(GL_ARRAY_BUFFER, vbo_vertices);
        glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), data.data(), GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*)(5 * sizeof(float)));

        glEnableVertexAttribArray(0);
        glBindVertexArray(0);

        return vao;
    }

    void Renderer::draw_scalar_field(u32 vao, glm::mat4 transform, glm::vec3 view_position, Texture const * const texture, u32 triangle_count)
    {
        sf_handle->data->use();
        sf_handle->data->set("proj", proj_mat_perspective);
        sf_handle->data->set("view", view_mat);
        sf_handle->data->set("model", transform);
        sf_handle->data->set("viewPos", view_position);
        sf_handle->data->set("text1", 0);

        bind_texture(texture, 0);

        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, triangle_count);
        glBindVertexArray(0);
    }


    u32 Renderer::load_primitive(glm::vec3* vertices, u32 vertices_count, u32* vertices_indices, u32 vertices_indices_count, glm::vec3* normals, u32 normals_count, u32* normals_indices, u32 normals_indices_count)
    {
        u32 vao, vbo_vertices, ebo_vertices_indices;
        //u32 vbo_normals, ebo_normals_indices;

        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        glGenBuffers(1, &vbo_vertices);
        glGenBuffers(1, &ebo_vertices_indices);

        glBindBuffer(GL_ARRAY_BUFFER, vbo_vertices);
        glBufferData(GL_ARRAY_BUFFER, vertices_count * sizeof(glm::vec3), vertices, GL_STATIC_DRAW);
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

    void Renderer::draw_primitive(const u32 vao, const u32 nindices, const Material& material, const glm::vec3& position, const glm::quat& orientation, const glm::vec3& scale)
    {
        //mat4 model = identity_mat4();
        //model = translate(model, position);

        material.shader.use();
        material.shader.set("model", glm::translate(glm::mat4(1), position) * glm::mat4_cast(orientation) * glm::scale(glm::mat4(1), scale));
        material.shader.set("proj", proj_mat_perspective);
        material.shader.set("view", view_mat);

        glBindVertexArray(vao);
        glEnableVertexAttribArray(0);
        //glEnableVertexAttribArray(1);

        glDrawElements(GL_TRIANGLES, nindices, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

    void Renderer::load_mesh(Mesh * mesh)
    {
        glGenVertexArrays(1, &mesh->vao);
        glGenBuffers(1, &mesh->vbo);
        glGenBuffers(1, &mesh->ebo);

        glBindVertexArray(mesh->vao);

        glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo);
        glBufferData(GL_ARRAY_BUFFER, mesh->vertices.size() * sizeof(Vertex), &mesh->vertices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->indices.size() * sizeof(unsigned int), &mesh->indices[0], GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));

        glBindVertexArray(0);
    }

    void Renderer::release_mesh(Mesh* mesh)
    {
        glDeleteBuffers(1, &mesh->vbo);
        glDeleteBuffers(1, &mesh->ebo);
        glDeleteVertexArrays(1, &mesh->vao);

        mesh->vbo = 0;
        mesh->ebo = 0;
        mesh->vao = 0;
    }

    void Renderer::draw_mesh(Mesh * mesh, const Material& material, const glm::vec3& position, const glm::quat& orientation, const glm::vec3& scale)
    {
        material.shader.use();
        material.shader.set("model", glm::translate(glm::mat4(1), position) * glm::mat4_cast(orientation) * glm::scale(glm::mat4(1), scale));
        material.shader.set("proj", proj_mat_perspective);
        material.shader.set("view", view_mat);

        glBindVertexArray(mesh->vao);
        glDrawElements(GL_TRIANGLES, mesh->indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        glActiveTexture(GL_TEXTURE0);
    }

    u32 Renderer::load_texture(const char* path, bool gamma)
    {
        u32 textureID;
        glGenTextures(1, &textureID);

        int width, height, nrComponents;
        unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
        if (data)
        {
            GLenum format;
            if (nrComponents == 1)
                format = GL_RED;
            else if (nrComponents == 3)
                format = GL_RGB;
            else if (nrComponents == 4)
                format = GL_RGBA;

            glBindTexture(GL_TEXTURE_2D, textureID);
            glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            stbi_image_free(data);
        }
        else
        {
            RAIN_LOG_ERROR("Texture failed to load from path: %s", path);
            stbi_image_free(data);
        }

        return textureID;
    }

    void Renderer::clear()
    {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void Renderer::draw_coord_view(const glm::vec3& _position)
    {
        draw_debug_line(glm::vec3{ 200.0f, 0.0f, 0.0f }, glm::vec3{ -200.0f, 0.0f, 0.0f }, glm::vec3{ 1.0f, 0.5f, 0.0f }, glm::vec3{ 0.5f, 1.0f, 0.0f });
        draw_debug_line(glm::vec3{ 0.0f, 200.0f, 0.0f }, glm::vec3{ 0.0f, -200.0f, 0.0f }, glm::vec3{ 0.0f, 1.0f, 0.5f }, glm::vec3{ 0.0f, 0.5f, 1.0f });
        draw_debug_line(glm::vec3{ 0.0f, 0.0f, 200.0f }, glm::vec3{ 0.0f, 0.0f, 0.0f }, glm::vec3{ 0.5f, 0.0f, 1.0f }, glm::vec3{ 1.0f, 0.0f, 0.5f });
    }


    void Renderer::init_shapes()
    {
        init_cube();
        init_sphere();
    }

    void Renderer::init_cube()
    {
        const float vertices[288] =
        {
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

    void Renderer::draw_quad(const core::plane& _p, const glm::vec3 _position, const glm::vec3& _color)
    {   
        glm::vec3 u = glm::cross(_p.n, _position); // cross product -> note that u lies on the plane
        glm::vec3 v = glm::cross(_p.n, u); // v is orthogonal to both N and u (again is in the plane)  

        glm::vec3 P0 = -_p.n * _p.D;        // "arbitrary" point
        float f      = 20.0f;  // large enough
        glm::vec3 fu = u * f;
        glm::vec3 fv = v * f;
        glm::vec3 A  = P0 - fu - fv;
        glm::vec3 B  = P0 + fu - fv;
        glm::vec3 C  = P0 + fu + fv;
        glm::vec3 D  = P0 - fu + fv;

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

        glm::mat4 mvp = proj_mat_perspective * view_mat * glm::mat4(1);
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

        std::vector<glm::vec3> positions;
        std::vector<glm::vec2> uv;
        std::vector<glm::vec3> normals;
        std::vector<unsigned int> indices;

        const unsigned int X_SEGMENTS = 32;
        const unsigned int Y_SEGMENTS = 32;
        const float PI = 3.14159265359f;
        for (unsigned int y = 0; y <= Y_SEGMENTS; ++y)
        {
            for (unsigned int x = 0; x <= X_SEGMENTS; ++x)
            {
                float xSegment = (float)x / (float)X_SEGMENTS;
                float ySegment = (float)y / (float)Y_SEGMENTS;
                float xPos = glm::cos(xSegment * 2.0f * PI) * glm::sin(ySegment * PI);
                float yPos = glm::cos(ySegment * PI);
                float zPos = glm::sin(xSegment * 2.0f * PI) * glm::sin(ySegment * PI);

                positions.push_back(glm::vec3{ xPos, yPos, zPos });
                uv.push_back(glm::vec2{ xSegment, ySegment });
                normals.push_back(glm::vec3{ xPos, yPos, zPos });
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

    void Renderer::init_transvoxel(const std::vector<transvoxel::tvox_vertex>& vertices, const std::vector<u32>& indices, u32& vao)
    {
        u32 vbo = 0;
        u32 ebo = 0;
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);
        glGenBuffers(1, &ebo);

        glBindVertexArray(vao);

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(transvoxel::tvox_vertex), vertices.data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(u32), indices.data(), GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(transvoxel::tvox_vertex), (void*)0);

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(transvoxel::tvox_vertex), (void*)offsetof(transvoxel::tvox_vertex, normal));

        //glEnableVertexAttribArray(2);
        //glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));

        glBindVertexArray(0);
    }

    void Renderer::update_transvoxel(const std::vector<glm::vec3>& vertices, const std::vector<u32>& indices, const u32& vao, const u32& vbo)
    {
        //std::vector<float> data;
        //data.reserve(2'000'000);
        //for (u32 i = 0; i < vertices.size(); ++i)
        //{
        //    data.push_back(vertices[i].x);
        //    data.push_back(vertices[i].y);
        //    data.push_back(vertices[i].z);

        //    data.push_back(normals[i].x);
        //    data.push_back(normals[i].y);
        //    data.push_back(normals[i].z);
        //}

        //glBindBuffer(GL_ARRAY_BUFFER, vbo);
        //glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), &data[0], GL_STATIC_DRAW);

        //i32 stride = (3 + 3) * sizeof(float);
        //glEnableVertexAttribArray(0);
        //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
        //glEnableVertexAttribArray(1);
        //glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));



        //glBindVertexArray(vao);
        //glBindBuffer(GL_ARRAY_BUFFER, vbo);
        //glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(glm::vec3), &vertices[0]);
        //glBufferSubData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), normals.size() * sizeof(glm::vec3), &normals[0]);

        //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);
        //glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)(vertices.size() * sizeof(glm::vec3)));
    }


    void Renderer::draw_transvoxel(const u32& vao, const u32 indices_count, const glm::vec3& view_position)
    {
        transvoxel_handle->data->use();
        transvoxel_handle->data->set("model", glm::mat4(1));
        transvoxel_handle->data->set("projection", proj_mat_perspective);
        transvoxel_handle->data->set("view", view_mat);
        transvoxel_handle->data->set("viewPos", view_position);

        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, indices_count, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        //glBindVertexArray(vao);
        //glDrawArrays(GL_TRIANGLES, 0, triangle_count);
        //glBindVertexArray(0);
    }


    void Renderer::init_instancing_cube(const std::vector<glm::mat4>& instances, u32& vao)
    {
        std::vector<glm::vec3> Positions = std::vector<glm::vec3>
        {
             glm::vec3(-0.5f, -0.5f, -0.5f),
             glm::vec3(0.5f,  0.5f, -0.5f),
             glm::vec3(0.5f, -0.5f, -0.5f),
             glm::vec3(0.5f,  0.5f, -0.5f),
             glm::vec3(-0.5f, -0.5f, -0.5f),
             glm::vec3(-0.5f,  0.5f, -0.5f),

             glm::vec3(-0.5f, -0.5f,  0.5f),
             glm::vec3(0.5f, -0.5f,  0.5f),
             glm::vec3(0.5f,  0.5f,  0.5f),
             glm::vec3(0.5f,  0.5f,  0.5f),
             glm::vec3(-0.5f,  0.5f,  0.5f),
             glm::vec3(-0.5f, -0.5f,  0.5f),

             glm::vec3(-0.5f,  0.5f,  0.5f),
             glm::vec3(-0.5f,  0.5f, -0.5f),
             glm::vec3(-0.5f, -0.5f, -0.5f),
             glm::vec3(-0.5f, -0.5f, -0.5f),
             glm::vec3(-0.5f, -0.5f,  0.5f),
             glm::vec3(-0.5f,  0.5f,  0.5f),

             glm::vec3(0.5f,  0.5f,  0.5f),
             glm::vec3(0.5f, -0.5f, -0.5f),
             glm::vec3(0.5f,  0.5f, -0.5f),
             glm::vec3(0.5f, -0.5f, -0.5f),
             glm::vec3(0.5f,  0.5f,  0.5f),
             glm::vec3(0.5f, -0.5f,  0.5f),

             glm::vec3(-0.5f, -0.5f, -0.5f),
             glm::vec3(0.5f, -0.5f, -0.5f),
             glm::vec3(0.5f, -0.5f,  0.5f),
             glm::vec3(0.5f, -0.5f,  0.5f),
             glm::vec3(-0.5f, -0.5f,  0.5f),
             glm::vec3(-0.5f, -0.5f, -0.5f),

             glm::vec3(-0.5f,  0.5f, -0.5f),
             glm::vec3(0.5f,  0.5f,  0.5f),
             glm::vec3(0.5f,  0.5f, -0.5f),
             glm::vec3(0.5f,  0.5f,  0.5f),
             glm::vec3(-0.5f,  0.5f, -0.5f),
             glm::vec3(-0.5f,  0.5f,  0.5f),
        };
        std::vector<glm::vec2> UV = std::vector<glm::vec2>
        {
            glm::vec2(0.0f, 0.0f),
            glm::vec2(1.0f, 1.0f),
            glm::vec2(1.0f, 0.0f),
            glm::vec2(1.0f, 1.0f),
            glm::vec2(0.0f, 0.0f),
            glm::vec2(0.0f, 1.0f),

            glm::vec2(0.0f, 0.0f),
            glm::vec2(1.0f, 0.0f),
            glm::vec2(1.0f, 1.0f),
            glm::vec2(1.0f, 1.0f),
            glm::vec2(0.0f, 1.0f),
            glm::vec2(0.0f, 0.0f),

            glm::vec2(1.0f, 0.0f),
            glm::vec2(1.0f, 1.0f),
            glm::vec2(0.0f, 1.0f),
            glm::vec2(0.0f, 1.0f),
            glm::vec2(0.0f, 0.0f),
            glm::vec2(1.0f, 0.0f),

            glm::vec2(1.0f, 0.0f),
            glm::vec2(0.0f, 1.0f),
            glm::vec2(1.0f, 1.0f),
            glm::vec2(0.0f, 1.0f),
            glm::vec2(1.0f, 0.0f),
            glm::vec2(0.0f, 0.0f),

            glm::vec2(0.0f, 1.0f),
            glm::vec2(1.0f, 1.0f),
            glm::vec2(1.0f, 0.0f),
            glm::vec2(1.0f, 0.0f),
            glm::vec2(0.0f, 0.0f),
            glm::vec2(0.0f, 1.0f),

            glm::vec2(0.0f, 1.0f),
            glm::vec2(1.0f, 0.0f),
            glm::vec2(1.0f, 1.0f),
            glm::vec2(1.0f, 0.0f),
            glm::vec2(0.0f, 1.0f),
            glm::vec2(0.0f, 0.0f),
        };
        std::vector<glm::vec3> Normals = std::vector<glm::vec3>
        {
            glm::vec3(0.0f,  0.0f, -1.0f),
            glm::vec3(0.0f,  0.0f, -1.0f),
            glm::vec3(0.0f,  0.0f, -1.0f),
            glm::vec3(0.0f,  0.0f, -1.0f),
            glm::vec3(0.0f,  0.0f, -1.0f),
            glm::vec3(0.0f,  0.0f, -1.0f),

            glm::vec3(0.0f,  0.0f,  1.0f),
            glm::vec3(0.0f,  0.0f,  1.0f),
            glm::vec3(0.0f,  0.0f,  1.0f),
            glm::vec3(0.0f,  0.0f,  1.0f),
            glm::vec3(0.0f,  0.0f,  1.0f),
            glm::vec3(0.0f,  0.0f,  1.0f),

            glm::vec3(-1.0f,  0.0f,  0.0f),
            glm::vec3(-1.0f,  0.0f,  0.0f),
            glm::vec3(-1.0f,  0.0f,  0.0f),
            glm::vec3(-1.0f,  0.0f,  0.0f),
            glm::vec3(-1.0f,  0.0f,  0.0f),
            glm::vec3(-1.0f,  0.0f,  0.0f),

            glm::vec3(1.0f,  0.0f,  0.0f),
            glm::vec3(1.0f,  0.0f,  0.0f),
            glm::vec3(1.0f,  0.0f,  0.0f),
            glm::vec3(1.0f,  0.0f,  0.0f),
            glm::vec3(1.0f,  0.0f,  0.0f),
            glm::vec3(1.0f,  0.0f,  0.0f),

            glm::vec3(0.0f, -1.0f,  0.0f),
            glm::vec3(0.0f, -1.0f,  0.0f),
            glm::vec3(0.0f, -1.0f,  0.0f),
            glm::vec3(0.0f, -1.0f,  0.0f),
            glm::vec3(0.0f, -1.0f,  0.0f),
            glm::vec3(0.0f, -1.0f,  0.0f),

            glm::vec3(0.0f,  1.0f,  0.0f),
            glm::vec3(0.0f,  1.0f,  0.0f),
            glm::vec3(0.0f,  1.0f,  0.0f),
            glm::vec3(0.0f,  1.0f,  0.0f),
            glm::vec3(0.0f,  1.0f,  0.0f),
            glm::vec3(0.0f,  1.0f,  0.0f),
        };


        std::vector<float> data;
        for (u32 i = 0; i < Positions.size(); ++i)
        {
            data.push_back(Positions[i].x);
            data.push_back(Positions[i].y);
            data.push_back(Positions[i].z);
            if (UV.size() > 0)
            {
                data.push_back(UV[i].x);
                data.push_back(UV[i].y);
            }
            if (Normals.size() > 0)
            {
                data.push_back(Normals[i].x);
                data.push_back(Normals[i].y);
                data.push_back(Normals[i].z);
            }
        }

        glGenVertexArrays(1, &vao);

        u32 cube_vbo, instance_vbo;
        glGenBuffers(1, &cube_vbo);
        glGenBuffers(1, &instance_vbo);

        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, cube_vbo);
        glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), &data[0], GL_STATIC_DRAW);

        i32 stride = (3 + 2 + 3) * sizeof(float);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*)(5 * sizeof(float)));

        glBindBuffer(GL_ARRAY_BUFFER, instance_vbo);
        glBufferData(GL_ARRAY_BUFFER, instances.size() * sizeof(glm::mat4), instances.data(), GL_STATIC_DRAW);

        // instance matrix
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
        glEnableVertexAttribArray(5);
        glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
        glEnableVertexAttribArray(6);
        glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));

        glVertexAttribDivisor(3, 1);
        glVertexAttribDivisor(4, 1);
        glVertexAttribDivisor(5, 1);
        glVertexAttribDivisor(6, 1);

        glBindVertexArray(0);
    }

    void Renderer::draw_instancing_cube(const u32 vao, const u32 amount, Texture const * const texture, const glm::vec3& view_position)
    {
        instancing_handle->data->use();
        instancing_handle->data->set("proj", proj_mat_perspective);
        instancing_handle->data->set("view", view_mat);
        instancing_handle->data->set("viewPos", view_position);
        instancing_handle->data->set("text1", 0);

        bind_texture(texture, 0);

        glBindVertexArray(vao);
        glDrawArraysInstanced(GL_TRIANGLES, 0, 36, amount);
        glBindVertexArray(0);
    }

    void Renderer::init_instancing(const std::vector<glm::mat4>& instances, u32& vao)
    {

        std::vector<glm::vec3> positions;
        std::vector<glm::vec2> uv;
        std::vector<glm::vec3> normals;
        std::vector<unsigned int> indices;

        const unsigned int X_SEGMENTS = 4;
        const unsigned int Y_SEGMENTS = 4;
        const float PI = 3.14159265359f;
        for (unsigned int y = 0; y <= Y_SEGMENTS; ++y)
        {
            for (unsigned int x = 0; x <= X_SEGMENTS; ++x)
            {
                float xSegment = (float)x / (float)X_SEGMENTS;
                float ySegment = (float)y / (float)Y_SEGMENTS;
                float xPos = glm::cos(xSegment * 2.0f * PI) * glm::sin(ySegment * PI);
                float yPos = glm::cos(ySegment * PI);
                float zPos = glm::sin(xSegment * 2.0f * PI) * glm::sin(ySegment * PI);

                positions.push_back(glm::vec3{ xPos, yPos, zPos });
                uv.push_back(glm::vec2{ xSegment, ySegment });
                normals.push_back(glm::vec3{ xPos, yPos, zPos });
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

        glGenVertexArrays(1, &vao);

        u32 sphere_vbo, sphere_ebo, instance_vbo;
        glGenBuffers(1, &sphere_vbo);
        glGenBuffers(1, &sphere_ebo);
        glGenBuffers(1, &instance_vbo);

        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, sphere_vbo);
        glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), &data[0], GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphere_ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
        i32 stride = (3 + 2 + 3) * sizeof(float);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*)(5 * sizeof(float)));

        glBindBuffer(GL_ARRAY_BUFFER, instance_vbo);
        glBufferData(GL_ARRAY_BUFFER, instances.size() * sizeof(glm::mat4), instances.data(), GL_STATIC_DRAW);

        // instance matrix
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
        glEnableVertexAttribArray(5);
        glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
        glEnableVertexAttribArray(6);
        glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));

        glVertexAttribDivisor(3, 1);
        glVertexAttribDivisor(4, 1);
        glVertexAttribDivisor(5, 1);
        glVertexAttribDivisor(6, 1);

        glBindVertexArray(0);
    }


    void Renderer::draw_instancing(const u32 vao, const u32 amount, Texture const * const texture, const glm::vec3& view_position)
    {
        instancing_handle->data->use();
        instancing_handle->data->set("proj", proj_mat_perspective);
        instancing_handle->data->set("view", view_mat);
        instancing_handle->data->set("viewPos", view_position);
        instancing_handle->data->set("text1", 0);

        bind_texture(texture, 0);
        glBindVertexArray(vao);
        glDrawElementsInstanced(GL_TRIANGLE_STRIP, sphere_index_count, GL_UNSIGNED_INT, 0, amount);
        glBindVertexArray(0);
    }



    void Renderer::init_debug()
    {
        glGenVertexArrays(1, &m_debug_vao);
        glBindVertexArray(m_debug_vao);

        glGenBuffers(1, &m_debug_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, m_debug_vbo);
        glBufferData(GL_ARRAY_BUFFER, debug_vertices_max_count * sizeof(glm::vec3), &m_debug_vertices[0], GL_DYNAMIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)nullptr);

        glGenBuffers(1, &m_debug_cbo);
        glBindBuffer(GL_ARRAY_BUFFER, m_debug_cbo);
        glBufferData(GL_ARRAY_BUFFER, debug_vertices_max_count * sizeof(glm::vec3), &m_debug_colors[0], GL_DYNAMIC_DRAW);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)nullptr);

        glEnableVertexAttribArray(0);
        glBindVertexArray(0);

        debug_shader.load(RAIN_CONFIG->data_root + "/shaders/glsl/debug.vert", RAIN_CONFIG->data_root + "/shaders/glsl/debug.frag");
    }

    void Renderer::draw()
    {
        draw_coord_view(glm::vec3 {});
		draw_debug();
		draw_ui();
        draw_text_2d();
    }

    void Renderer::draw_debug()
    {
        if (m_debug_vertex_count == 0)
        {
            return;
        }

        glBindBuffer(GL_ARRAY_BUFFER, m_debug_vbo);
        glBufferSubData(GL_ARRAY_BUFFER, 0, m_debug_vertex_count * sizeof(glm::vec3), m_debug_vertices);

        glBindBuffer(GL_ARRAY_BUFFER, m_debug_cbo);
        glBufferSubData(GL_ARRAY_BUFFER, 0, m_debug_vertex_count * sizeof(glm::vec3), m_debug_colors);

        glDisable(GL_BLEND);

        glm::mat4 mvp = proj_mat_perspective * view_mat * glm::mat4(1);
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

    void Renderer::draw_debug_line(const glm::vec3& _point1, const glm::vec3& _point2, const glm::vec3& _color)
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

    void Renderer::draw_debug_line(const glm::vec3& _point1, const glm::vec3& _point2, const glm::vec3& _color1, const glm::vec3& _color2)
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

    void Renderer::draw_debug_cube(const glm::vec3& _center, const f32 _width, const f32 _height, const glm::vec3& _color)
    {
        float halfWidth = _width / 2.0f;
        float halfHeight = _height / 2.0f;

        glm::vec3 A{ _center.x - halfWidth, _center.y + halfWidth, _center.z - halfHeight };
        glm::vec3 B{ _center.x + halfWidth, _center.y + halfWidth, _center.z - halfHeight };
        glm::vec3 C{ _center.x + halfWidth, _center.y - halfWidth, _center.z - halfHeight };
        glm::vec3 D{ _center.x - halfWidth, _center.y - halfWidth, _center.z - halfHeight };
        glm::vec3 E{ _center.x - halfWidth, _center.y + halfWidth, _center.z + halfHeight };
        glm::vec3 F{ _center.x + halfWidth, _center.y + halfWidth, _center.z + halfHeight };
        glm::vec3 G{ _center.x + halfWidth, _center.y - halfWidth, _center.z + halfHeight };
        glm::vec3 H{ _center.x - halfWidth, _center.y - halfWidth, _center.z + halfHeight };

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

    void Renderer::draw_debug_sphere(const glm::vec3& _position, const f32 _scale, const glm::quat& orientation)
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

    //void Renderer::draw_quad(const core::glm::Plane& _p, const vec3 _position, const vec3& _color)
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

    void Renderer::draw_cube(const glm::vec3& position, const glm::quat& orientation, const glm::vec3& scale)
    {
        phong_handle->data->use();
        phong_handle->data->set("model", glm::translate(glm::mat4(1), position) * glm::mat4_cast(orientation) * glm::scale(glm::mat4(1), scale));
        phong_handle->data->set("proj", proj_mat_perspective);
        phong_handle->data->set("view", view_mat);

        glBindVertexArray(cubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
    }

    void Renderer::draw_sphere(const glm::vec3& position, const glm::quat& orientation, const glm::vec3& scale)
    {
        phong_handle->data->use();
        phong_handle->data->set("model", glm::translate(glm::mat4(1), position) * glm::mat4_cast(orientation) * glm::scale(glm::mat4(1), scale));
        phong_handle->data->set("proj", proj_mat_perspective);
        phong_handle->data->set("view", view_mat);

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
        texts.reserve(128);
        text_shader.load(RAIN_CONFIG->data_root + "/shaders/glsl/text_2d.vert", RAIN_CONFIG->data_root + "/shaders/glsl/text_2d.frag");

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

    void Renderer::draw_text_2d()
    {
        glEnable(GL_CULL_FACE);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        text_shader.use();
        text_shader.set("projection", proj_mat_orthogonal);
        glActiveTexture(GL_TEXTURE0);
        glBindVertexArray(text_vao);

        for (u32 i = 0; i < texts.size(); ++i)
        {
            text_shader.set("textColor", texts[i].color);

            std::string::const_iterator c;
            GLfloat x = texts[i].x;
            for (c = texts[i].value.begin(); c != texts[i].value.end(); c++)
            {
                RChar ch = text_characters[*c];

                GLfloat xpos = x + ch.bearing[0] * texts[i].scale;
                GLfloat ypos = texts[i].y - (ch.size[1] - ch.bearing[1]) * texts[i].scale;

                GLfloat w = ch.size[0] * texts[i].scale;
                GLfloat h = ch.size[1] * texts[i].scale;

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

                x += (ch.advance >> 6) * texts[i].scale;
            }
        }
        texts.clear();
        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D, 0);
        //glDisable(GL_CULL_FACE);
    }

    void Renderer::draw_text_2d(const std::string& _text, const f32 _x, const f32 _y, const f32 _scale, const glm::vec4& _color)
    {
        texts.push_back(text{_text, _x, _y, _scale, _color});
    }

	void Renderer::init_ui()
	{
		glGenVertexArrays(1, &ui_vao);
		glBindVertexArray(ui_vao);

		glGenBuffers(1, &ui_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, ui_vbo);
		glBufferData(GL_ARRAY_BUFFER, ui_max_vertex_count * sizeof(glm::vec3), &ui_vertices[0], GL_DYNAMIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)nullptr);

		glGenBuffers(1, &ui_cbo);
		glBindBuffer(GL_ARRAY_BUFFER, ui_cbo);
		glBufferData(GL_ARRAY_BUFFER, ui_max_vertex_count * sizeof(glm::vec3), &ui_colors[0], GL_DYNAMIC_DRAW);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)nullptr);

		glEnableVertexAttribArray(0);
		glBindVertexArray(0);

		ui_shader.load(RAIN_CONFIG->data_root + "/shaders/glsl/ui.vert", RAIN_CONFIG->data_root + "/shaders/glsl/ui.frag");
	}

	void Renderer::draw_ui()
	{
		if (ui_vertex_count == 0)
		{
			return;
		}

		glBindBuffer(GL_ARRAY_BUFFER, ui_vbo);
		glBufferSubData(GL_ARRAY_BUFFER, 0, ui_vertex_count * sizeof(glm::vec3), ui_vertices);

		glBindBuffer(GL_ARRAY_BUFFER, ui_cbo);
		glBufferSubData(GL_ARRAY_BUFFER, 0, ui_vertex_count * sizeof(glm::vec3), ui_colors);

		ui_shader.use();
		ui_shader.set("projection", proj_mat_orthogonal);

		glBindVertexArray(ui_vao);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);

		glDrawArrays(GL_TRIANGLES, 0, ui_vertex_count);

		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(0);
		glBindVertexArray(0);

		ui_vertex_count = 0;
	}

	void Renderer::draw_ui_triangle(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c, const glm::vec4& a_color)
	{
		draw_ui_triangle(a, b, c, a_color, a_color, a_color);
	}

	void Renderer::draw_ui_triangle(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c, const glm::vec4& a_color, const glm::vec4& b_color)
	{
		draw_ui_triangle(a, b, c, a_color, b_color, a_color);
	}

	void Renderer::draw_ui_triangle(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c, const glm::vec4& a_color, const glm::vec4& b_color, const glm::vec4& c_color)
	{
		if (m_debug_vertex_count + 3 < debug_vertices_max_count)
		{
			ui_vertices[ui_vertex_count + 0] = a;
			ui_vertices[ui_vertex_count + 1] = b;
			ui_vertices[ui_vertex_count + 2] = c;
            
            ui_colors[ui_vertex_count + 0] = glm::vec3{ a_color.x, a_color.y, a_color.z };
            ui_colors[ui_vertex_count + 1] = glm::vec3{ b_color.x, b_color.y, b_color.z };
            ui_colors[ui_vertex_count + 2] = glm::vec3{ c_color.x, c_color.y, c_color.z };

			ui_vertex_count += 3;
		}
	}

    void Renderer::draw_ui_quad(const f32 x_bottom_left, const f32 y_bottom_left, const f32& width, const f32& height, const glm::vec4& color)
    {
        glm::vec3 bottom_left{ x_bottom_left, y_bottom_left, 0.0f };
        glm::vec3 bottom_right{ x_bottom_left + width, y_bottom_left, 0.0f };
        glm::vec3 top_left{ x_bottom_left, y_bottom_left + height, 0.0f };
        glm::vec3 top_right{ x_bottom_left + width, y_bottom_left + height, 0.0f };

        draw_ui_triangle(top_left, bottom_left, top_right, color);
        draw_ui_triangle(bottom_left, bottom_right, top_right, color);
    }
}