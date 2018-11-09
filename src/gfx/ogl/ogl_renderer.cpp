#include "ogl_renderer.h"

#include "stdio.h"
#include <iostream>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/trigonometric.hpp>

#include "core/high_resolution_clock.h"
#include "ogl_shader.h"

namespace rain
{
    void Renderer::init()
    {
        GLenum res = glewInit();
        char buffer[500];
        sprintf_s(buffer, 500, "Console working\n");
        OutputDebugString(buffer);

        if (res != GLEW_OK)
        {
            sprintf_s(buffer, 500, "Error : %s\n", glewGetErrorString(res));
            OutputDebugString(buffer);
        }

        glEnable(GL_TEXTURE_2D);
        glShadeModel(GL_SMOOTH);
        glClearColor(0.1f, 0.1f, 0.1f, 0.0f);
        glClearDepth(1.0f);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
        glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
        glViewport(0, 0, 800, 600);

        set_projection_matrix(glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 10000.0f));    

        init_default_shaders();

        init_coord_view();
        init_quad();
        init_cube();
        init_sphere();
    }

    void Renderer::init_default_shaders()
    {
        shaders_indexer_path = new DataIndexer(RAIN_DATA.root + "/engine/shaders/glsl/glsl_shader_index.rain");

        // creating default phong shader
        std::string default_phong_shaders = shaders_indexer_path->find("default_phong");
        auto shaders = String::pair_split(default_phong_shaders, " ");
        bool retval = default_phong.load(shaders_indexer_path->file.get_directory() + shaders.first, shaders_indexer_path->file.get_directory() + shaders.second);
        assert(retval);
        std::string default_coord_shaders = shaders_indexer_path->find("default_coord_view");
        shaders = String::pair_split(default_coord_shaders, " ");
        retval = default_coord_view.load(shaders_indexer_path->file.get_directory() + shaders.first, shaders_indexer_path->file.get_directory() + shaders.second);
        assert(retval);
    }

    void Renderer::init_coord_view()
    {
        float coord_lines[36] =
        {
             0.0, 0.0, 0.0,  1.0, 0.0, 0.0,
             20.0, 0.0, 0.0,  1.0, 0.0, 0.0,
             0.0, 0.0, 0.0,  0.0, 1.0, 0.0,
             0.0, 20.0, 0.0,  0.0, 1.0, 0.0,
             0.0, 0.0, 20.0,  0.0, 0.0, 1.0,
             0.0, 0.0, 0.0,  0.0, 0.0, 1.0,
        };

        glGenVertexArrays(1, &coordviewVAO);
        glBindVertexArray(coordviewVAO);

        unsigned int coordviewVBO;
        glGenBuffers(1, &coordviewVBO);
        glBindBuffer(GL_ARRAY_BUFFER, coordviewVBO);

        glBufferData(GL_ARRAY_BUFFER, sizeof(coord_lines), coord_lines, GL_STATIC_DRAW);

        // position
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);


        //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        //glEnableVertexAttribArray(0);
        //// normals
        //glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        //glEnableVertexAttribArray(1);

        glBindVertexArray(0);
    }

    void Renderer::init_quad()
    {
        //float vertices[] = {
        //     0.5f,  0.5f, 0.0f,
        //     0.5f, -0.5f, 0.0f,
        //    -0.5f, -0.5f, 0.0f,
        //    -0.5f,  0.5f, 0.0f 
        //};
        //unsigned int indices[] =
        //{
        //    0, 1, 3,
        //    1, 2, 3
        //};
        //unsigned int VBO, EBO;
        //glGenVertexArrays(1, &VAO);
        //glGenBuffers(1, &VBO);
        //glGenBuffers(1, &EBO);
        //glBindVertexArray(VAO);

        //glBindBuffer(GL_ARRAY_BUFFER, VBO);
        //glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        //glEnableVertexAttribArray(0);

        //glBindBuffer(GL_ARRAY_BUFFER, 0);
        //glBindVertexArray(0);
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
	    const float PI = 3.14159265359;
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
	    for (int i = 0; i < positions.size(); ++i)
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
	    float stride = (3 + 2 + 3) * sizeof(float);
	    glEnableVertexAttribArray(0);
	    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
	    glEnableVertexAttribArray(1);
	    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
	    glEnableVertexAttribArray(2);
	    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*)(5 * sizeof(float)));
    }


    void Renderer::render_sphere(const glm::vec3& _position, const glm::quat& orientation)
    {
        default_phong.use();
	    glm::mat4 mvp = projection * view_mat * glm::translate(glm::mat4(1), _position) * glm::mat4_cast(orientation);
        default_phong.set("mvp", glm::value_ptr(mvp));
    

	    glBindVertexArray(sphereVAO);
	    glDrawElements(GL_TRIANGLE_STRIP, sphere_index_count, GL_UNSIGNED_INT, 0);
	    glBindVertexArray(0);

    }

    void Renderer::set_projection_matrix(const glm::mat4& _projection)
    {
        projection = _projection;
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

    void Renderer::render_quad()
    {
        //glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);
        //glm::mat4 trans = glm::mat4(1);
        //trans = glm::scale(trans, glm::vec3(0.1f,0.1f,0.1f));

        //unsigned int transformLoc = glGetUniformLocation(shaderProgram, "transform");
        //glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));

        //glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        //glClear(GL_COLOR_BUFFER_BIT);

        //glUseProgram(shaderProgram);
        //glBindVertexArray(VAO);
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }


    void Renderer::render_coord_view(const glm::vec3& _position)
    {
        default_coord_view.use();
        glm::mat4 mvp = projection * view_mat * glm::translate(glm::mat4(1), _position);
        default_coord_view.set("mvp", mvp);

        glBindVertexArray(coordviewVAO);
        glDrawArrays(GL_LINES, 0, 9);
        glBindVertexArray(0);
    }


    void Renderer::render_cube(const glm::vec3& _position, const glm::quat& orientation)
    {
        default_phong.use();
        glm::mat4 mvp = projection * view_mat * glm::translate(glm::mat4(1), _position) * glm::mat4_cast(orientation);
        default_phong.set("mvp", mvp);

        glBindVertexArray(cubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        //glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }
}