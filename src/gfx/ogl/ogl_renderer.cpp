#include "ogl_renderer.h"

#include "stdio.h"
#include <iostream>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtc/quaternion.hpp>



#include "core/high_resolution_clock.h"
#include "ogl_shader.h"
#include "win32/win32_application.h"
#include "win32/win32_window.h"
#include "gfx/gfx_camera.h"

#include "core/log.h"

#include <ft2build.h>
#include FT_FREETYPE_H


namespace rain
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
        glDepthFunc(GL_LEQUAL);
        glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
        resize(RAIN_WINDOW->size().x, RAIN_WINDOW->size().y);

        init_default_shaders();
        init_debug();
        init_shapes();
        init_text_2d();

        camera = new Camera();
        camera->init();
    }

    void Renderer::resize(u32 _width, u32 _height)
    {
        glViewport(0, 0, _width, _height);
        set_perspective_projection_matrix(glm::perspective(glm::radians(45.0f), (f32)_width / (f32)_height, 0.1f, 10000.0f));
        set_orthogonal_projection_matrix(glm::ortho(0.0f, (f32)_width, 0.0f, (f32)_height));
        
    }

    void Renderer::update_camera()
    {
        camera->update();
        set_view_matrix(camera->position, camera->position + camera->front, camera->up);
    }

    void Renderer::init_default_shaders()
    {
        bool retval = default_phong.load(RAIN_CONFIG->data_root + "/shaders/glsl/phong.vs", RAIN_CONFIG->data_root + "/shaders/glsl/phong.fs");

        default_phong.use();
        default_phong.set("lightDiff", 0.3f, 0.3f, 0.3f);
        default_phong.set("lightDirection", -0.2f, -1.0f, -0.3f);
    }

    void Renderer::set_perspective_projection_matrix(const glm::mat4& _projection)
    {
        proj_mat_perspective = _projection;
    }

    void Renderer::set_orthogonal_projection_matrix(const glm::mat4& _projection)
    {
        proj_mat_orthogonal = _projection;
        proj_mat_orthogonal = glm::ortho(0.0f, static_cast<GLfloat>(800), 0.0f, static_cast<GLfloat>(600));
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

        view_mat = {
            glm::vec4(xaxis.x,            yaxis.x,            zaxis.x,      0),
            glm::vec4(xaxis.y,            yaxis.y,            zaxis.y,      0),
            glm::vec4(xaxis.z,            yaxis.z,            zaxis.z,      0),
            glm::vec4(-glm::dot(xaxis, _eye), -glm::dot(yaxis, _eye), -glm::dot(zaxis, _eye), 1)
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

    void Renderer::clear()
    {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void Renderer::draw_coord_view(const glm::vec3& _position)
    {
        draw_debug_line(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(20.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        draw_debug_line(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 20.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        draw_debug_line(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 20.0f), glm::vec3(0.0f, 0.0f, 1.0f));
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

        const unsigned int X_SEGMENTS = 64;
        const unsigned int Y_SEGMENTS = 64;
        const float PI = 3.14159265359f;
        for (unsigned int y = 0; y <= Y_SEGMENTS; ++y)
        {
            for (unsigned int x = 0; x <= X_SEGMENTS; ++x)
            {
                float xSegment = (float)x / (float)X_SEGMENTS;
                float ySegment = (float)y / (float)Y_SEGMENTS;
                float xPos = std::cos(xSegment * 2.0f * PI) * std::sin(ySegment * PI);
                float yPos = std::cos(ySegment * PI);
                float zPos = std::sin(xSegment * 2.0f * PI) * std::sin(ySegment * PI);

                positions.push_back(glm::vec3(xPos, yPos, zPos));
                uv.push_back(glm::vec2(xSegment, ySegment));
                normals.push_back(glm::vec3(xPos, yPos, zPos));
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
        glBufferData(GL_ARRAY_BUFFER, debug_vertices_max_count * sizeof(glm::vec3), &m_debug_vertices[0], GL_DYNAMIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)nullptr);

        glGenBuffers(1, &m_debug_cbo);
        glBindBuffer(GL_ARRAY_BUFFER, m_debug_cbo);
        glBufferData(GL_ARRAY_BUFFER, debug_vertices_max_count * sizeof(glm::vec3), &m_debug_colors[0], GL_DYNAMIC_DRAW);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)nullptr);

        glEnableVertexAttribArray(0);
        glBindVertexArray(0);

        debug_shader.load(RAIN_CONFIG->data_root + "/shaders/glsl/debug.vs", RAIN_CONFIG->data_root + "/shaders/glsl/debug.fs");
    }

    void Renderer::draw()
    {
        draw_coord_view(glm::vec3(0.0f, 0.0f, 0.0f));
        draw_debug();
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

        glEnable(GL_MULTISAMPLE);
        glDisable(GL_BLEND);

        glm::mat4 mvp = proj_mat_perspective * view_mat * glm::mat4(1.0f);
        debug_shader.use();
        debug_shader.set("mvp", mvp);

        glBindVertexArray(m_debug_vao);
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);

        glDrawArrays(GL_LINES, 0, m_debug_vertex_count);

        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(0);
        glBindVertexArray(0);

        glDisable(GL_MULTISAMPLE);

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

    void Renderer::draw_debug_cube(const glm::vec3& _center, const f32 _width, const f32 _height, const glm::vec3& _color)
    {
        float halfWidth = _width / 2.0f;
        float halfHeight = _height / 2.0f;

        glm::vec3 A(_center.x - halfWidth, _center.y + halfWidth, _center.z - halfHeight);
        glm::vec3 B(_center.x + halfWidth, _center.y + halfWidth, _center.z - halfHeight);
        glm::vec3 C(_center.x + halfWidth, _center.y - halfWidth, _center.z - halfHeight);
        glm::vec3 D(_center.x - halfWidth, _center.y - halfWidth, _center.z - halfHeight);
        glm::vec3 E(_center.x - halfWidth, _center.y + halfWidth, _center.z + halfHeight);
        glm::vec3 F(_center.x + halfWidth, _center.y + halfWidth, _center.z + halfHeight);
        glm::vec3 G(_center.x + halfWidth, _center.y - halfWidth, _center.z + halfHeight);
        glm::vec3 H(_center.x - halfWidth, _center.y - halfWidth, _center.z + halfHeight);

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

    void Renderer::draw_quad()
    {
        assert(false);
    }

    void Renderer::draw_cube(const glm::vec3& _position, const f32 _scale,  const glm::quat& _orientation)
    {
        default_phong.use();
        default_phong.set("model", glm::translate(glm::mat4(1), _position) * glm::mat4_cast(_orientation) * glm::scale(glm::mat4(1), glm::vec3(_scale)));
        default_phong.set("proj", proj_mat_perspective);
        default_phong.set("view", view_mat);


        glBindVertexArray(cubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
    }

    void Renderer::draw_sphere(const glm::vec3& _center, const f32 _scale, const glm::quat& orientation)
    {
        default_phong.use();
        default_phong.set("model", glm::translate(glm::mat4(1), _center) * glm::mat4_cast(orientation) * glm::scale(glm::mat4(1), glm::vec3(_scale)));
        default_phong.set("proj", proj_mat_perspective);
        default_phong.set("view", view_mat);

        glBindVertexArray(sphereVAO);
        glDrawElements(GL_TRIANGLE_STRIP, sphere_index_count, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
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
            printf("ERROR::FREETYPE: Could not init FreeType Library\n");
            return;
        }

        std::string path_to_font = RAIN_CONFIG->data_root + "/fonts/arial.ttf";
        FT_Face face;
        if (FT_New_Face(ft, path_to_font.c_str(), 0, &face))
        {
            printf("ERROR::FREETYPE: Failed to load font\n");
            return;
        }

        FT_Set_Pixel_Sizes(face, 0, 48);

        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        for (GLubyte c = 0; c < 128; c++)
        {
            if (FT_Load_Char(face, c, FT_LOAD_RENDER))
            {
                std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
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
                glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
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

    void Renderer::draw_text_2d(const std::string& _text, const GLfloat _x, const GLfloat _y, const GLfloat _scale, const glm::vec3& _color)
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

            GLfloat xpos = x + ch.bearing.x * _scale;
            GLfloat ypos = _y - (ch.size.y - ch.bearing.y) * _scale;

            GLfloat w = ch.size.x * _scale;
            GLfloat h = ch.size.y * _scale;

            GLfloat vertices[6][4] = {
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