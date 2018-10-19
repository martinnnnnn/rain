#include "ogl_renderer.h"

#include "stdio.h"
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/trigonometric.hpp>

#include "core/high_resolution_clock.h"

void Renderer::initialize()
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

    initialize_coord_view();
    initialize_quad();
    initialize_cube();
}

void Renderer::initialize_coord_view()
{
    const char *vertexShaderSource = "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "layout(location = 1) in vec3 aColor;\n"
        "out vec4 color;"
        "uniform mat4 mvp;\n"
        "void main()\n"
        "{\n"
        "   gl_Position = mvp * vec4(aPos, 1.0f);\n"
        "   color = vec4(aColor, 1.0f);\n"
        "}\0";
    const char *fragmentShaderSource = "#version 330 core\n"
        "out vec4 FragColor;\n"
        "in vec4 color;\n"
        "void main()\n"
        "{\n"
        "   FragColor = color;\n"
        "}\n\0";

    int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        char buffer[512];
        sprintf_s(buffer, "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n%s\n", infoLog);
        OutputDebugStringA(buffer);
    }
    // fragment shader
    int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        char buffer[512];
        sprintf_s(buffer, "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n%s\n", infoLog);
        OutputDebugStringA(buffer);
    }
    // link shaders
    coordviewShaderProgram = glCreateProgram();
    glAttachShader(coordviewShaderProgram, vertexShader);
    glAttachShader(coordviewShaderProgram, fragmentShader);
    glLinkProgram(coordviewShaderProgram);
    // check for linking errors
    glGetProgramiv(coordviewShaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(coordviewShaderProgram, 512, NULL, infoLog);
        char buffer[512];
        sprintf_s(buffer, "ERROR::SHADER::PROGRAM::LINKING_FAILED\n%s\n", infoLog);
        OutputDebugStringA(buffer);
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);


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

void Renderer::initialize_quad()
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

void Renderer::initialize_cube()
{
    const char *vertexShaderSource = "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "uniform mat4 mvp;\n"
        "void main()\n"
        "{\n"
        "   gl_Position = mvp * vec4(aPos, 1.0f);"
        "}\0";
    const char *fragmentShaderSource = "#version 330 core\n"
        "out vec4 FragColor;\n"
        "void main()\n"
        "{\n"
        "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
        "}\n\0";

    int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        char buffer[512];
        sprintf_s(buffer, "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n%s\n", infoLog);
        OutputDebugStringA(buffer);
    }
    // fragment shader
    int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        char buffer[512];
        sprintf_s(buffer, "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n%s\n", infoLog);
        OutputDebugStringA(buffer);
    }
    // link shaders
    cubeShaderProgram = glCreateProgram();
    glAttachShader(cubeShaderProgram, vertexShader);
    glAttachShader(cubeShaderProgram, fragmentShader);
    glLinkProgram(cubeShaderProgram);
    // check for linking errors
    glGetProgramiv(cubeShaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(cubeShaderProgram, 512, NULL, infoLog);
        char buffer[512];
        sprintf_s(buffer, "ERROR::SHADER::PROGRAM::LINKING_FAILED\n%s\n", infoLog);
        OutputDebugStringA(buffer);
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

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
    glm::mat4 mvp = projection * view_mat * glm::translate(glm::mat4(1), _position);

    glBindVertexArray(coordviewVAO);
    glUseProgram(coordviewShaderProgram);

    unsigned int transformLoc = glGetUniformLocation(coordviewShaderProgram, "mvp");
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(mvp));

    glDrawArrays(GL_LINES, 0, 9);
}


void Renderer::render_cube(const glm::vec3& _position)
{
    glm::mat4 mvp = projection * view_mat * glm::translate(glm::mat4(1), _position);

    glBindVertexArray(cubeVAO);
    glUseProgram(cubeShaderProgram);

    unsigned int transformLoc = glGetUniformLocation(cubeShaderProgram, "mvp");
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(mvp));

    glDrawArrays(GL_TRIANGLES, 0, 36);
    //glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Renderer::draw_triangle()
{

}