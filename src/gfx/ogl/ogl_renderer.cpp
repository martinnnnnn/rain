#include "ogl_renderer.h"

#include "ogl_window.h"

int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE hprevinstance, LPSTR lpcmdline, int nshowcmd)
{
    rain::OGLRenderer renderer;
    rain::OGLInitRenderer(&renderer, hinstance);
    rain::OGLRunRenderer(&renderer);
    return 0;
}


namespace rain
{
    void OGLInitRenderer(OGLRenderer* _renderer, HINSTANCE _hinstance)
    {
        _renderer->window = (OGLWindow*)calloc(1, sizeof(OGLWindow));
        OGL_WINDOW_DESC desc;
        desc.hinstance = _hinstance;
        desc.width = 800;
        desc.height = 600;
        desc.depth = 16;
        desc.fullscreen = false;
        desc.WndClassName = L"rain";
        desc.WndName = L"Rain Engine";
        
        OGLInitWindow(&desc, _renderer->window);
    }

    void OGLRunRenderer(OGLRenderer* _renderer)
    {
        MSG msg;
        OGLWindow* window = _renderer->window;
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
            InvalidateRect(window->hwnd, NULL, TRUE);


            if (GetAsyncKeyState(VK_ESCAPE))
                OGLShutdown(window);
        }
    }

    void OGLUpdate(OGLRenderer* _renderer)
    {

    }

    void OGLRender(OGLRenderer* _renderer)
    {

    }

    //GLuint OGLLoadVerticesToGPU(float* vertices)
    //{
    //    GLuint cubeVBO;
    //    GLuint cubeVAO;

    //    glGenBuffers(1, &cubeVBO);
    //    glGenVertexArrays(1, &cubeVAO);

    //    glBindVertexArray(cubeVAO);

    //    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    //    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //    // position
    //    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    //    glEnableVertexAttribArray(0);
    //    // normals
    //    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    //    glEnableVertexAttribArray(1);
    //    // texture coord attribute
    //    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    //    glEnableVertexAttribArray(2);

    //    glBindVertexArray(0);

    //    return cubeVAO;
    //}

    //void OGLBindVAO(GLuint _vao)
    //{
    //    glBindVertexArray(_vao);
    //}

    //void OGLDrawTriangles(u32 _indicesCount)
    //{
    //    glDrawArrays(GL_TRIANGLES, 0, _indicesCount);
    //}
}

