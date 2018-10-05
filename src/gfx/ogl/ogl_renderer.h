#pragma once

#include <Windows.h>
#include <glad/glad.h> 
#include <gl/GL.h>
#include <gl/GLU.h>
#include <vector>

#include "gfx/gfx_model.h"
#include "gfx/gfx_light.h"

namespace rain
{
    struct OGLWindow;

    struct OGLRenderer
    {
        OGLWindow* window;
    };

    void OGLInitRenderer(OGLRenderer* _application, HINSTANCE _hinstance);
    void OGLRunRenderer(OGLRenderer* _renderer);
    void OGLUpdate(OGLRenderer* _application);
    void OGLRender(OGLRenderer* _application);

    struct OGLScene
    {
        Camera camera;
        std::vector<Model> models;
        std::vector<Light> lights;
    };


    void OGLUpdateScene(OGLScene* _scene);
    void OGLDrawScene(OGLScene* _scene);



    //GLuint OGLLoadVerticesToGPU(float* vertices);
    //void OGLBindVAO(GLuint _vao);
    //void OGLDrawTriangles(u32 _indicesCount);
}

