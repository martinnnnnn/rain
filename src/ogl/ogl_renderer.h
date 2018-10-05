#pragma once

#include <Windows.h>

#include "GL/glew.h"
#include "gl/GL.h"
#include "gl/GLU.h"
#include "glm/glm.hpp"

class Shader;

struct StaticMesh
{
    GLuint vao;
};

class Renderer
{
public:
    void initialize();

    void clear();

    void render();

    void draw_triangle();

    GLuint VAO;
    unsigned int shaderProgram;
};