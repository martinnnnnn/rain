#include <iostream>
#include <fstream>
#include <windows.h>
#include <string>
#include <sstream>
#include <vector>
#include <iterator>
#include <unordered_map>
#include <nlohmann/json.hpp>

#include "utility/incl_3d.h"
#include "rain.h"
#include "gfx/gfx_shader.h"
#include "gfx/gfx_camera.h"
#include "core/transform.h"
#include "input/input.h"
#include "gfx/gfx_light.h"
#include "utility/gl_utils.h"
#include "gfx/gfx_material.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include "gfx/gfx_model.h"
#include "gfx/gfx_texture.h"
#include "utility/string_utils.h"
#include "core/memory.h"
#include "memory/mem_resource_manager.h"

using namespace rain;

std::string rootpath;
bool wireframe;
Shader shaderProgram;
Shader lightshaderProgramRed;
Shader lightshaderProgramBlue;
Shader lightshaderProgramYellow;
Shader outlineShaderProgram;
Shader screenquadShader;
Shader skyboxShader;
GLuint quadVAO, quadVBO;
GLuint frameBuffer;
GLuint textureColorBuffer;
GLuint boxVBO;
GLuint boxVAO;
GLuint lightVBO;
GLuint lightVAO;
GLuint uboMatrices;
Texture2D container2Diffuse;
Texture2D container2Specular;
Texture2D container2Emissive;
GLuint skyboxVAO;
GLuint skyboxTexture;
glm::vec3 lightPos;
std::vector<glm::vec3> cubePositions;
Model model;
Game* game;
Transform modelTransform;
bool vsync;

//Entity* entity;


void sandboxInit();
void sandboxUpdate();
void checkInputs();
void loadShaders();

#define RAIN_DLLEXPORT extern "C" __declspec(dllexport)

RAIN_DLLEXPORT void LoadGame(const char* path)
{




    vsync = true;
    game = InitGame(path);
    //Transform* camTransform = game->camera->GetTransform();
    Transform* camTransform = &game->camera->transform;
    Translate(*camTransform, glm::vec3(0, 0, 5));

    rootpath = game->dataPath;
    wireframe = false;

    sandboxInit();
    loadShaders();

    RunGame(game, sandboxUpdate);
}

RAIN_DLLEXPORT void OpenWindow(void* wndHandle)
{
    ////TCHAR* AppClassName = _T("Windows Cipher");
    //HACCEL hAccel = 0;												// no accelerator table in this example.
    //MSG Msg;
    //RECT R;
    //HWND Wnd = (HWND)wndHandle;											
    //
    //SetClassLongPtr(Wnd, GCL_WNDPROC, reinterpret_cast<LONG_PTR>(OpenGLMDIChildHandler));
    //GetClientRect(GetDesktopWindow(), &R);							
    //while (GetMessage(&Msg, 0, 0, 0))
    //{
    //    TranslateMessage(&Msg);									
    //    DispatchMessage(&Msg);									
    //};
}

RAIN_DLLEXPORT void EnableVsync()
{
    vsync = true;
}

RAIN_DLLEXPORT void DisableVsync()
{
    vsync = false;
}

void runEngine(int argc, char** argv);

int main(int argc, char** argv)
{
    runEngine(argc, argv);


    return 0;
}

void runEngine(int argc, char** argv)
{
    game = InitGame(argc, argv);
    Transform* camTransform = &game->camera->transform;
    Translate(*camTransform, glm::vec3(0, 0, 5));

    rootpath = game->dataPath;
    wireframe = false;

    sandboxInit();
    loadShaders();
    RunGame(game, sandboxUpdate);
    glfwSwapInterval(0);
}

void sandboxInit()
{
    Scale(modelTransform, glm::vec3(0.3, 0.3, 0.3));

    float vertices[] = {
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

    GLuint indices[] =
    {
        0, 1, 3,
        1, 2, 3 
    };

    cubePositions = 
    {
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(2.0f, 5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3(2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f, 3.0f, -7.5f),
        glm::vec3(1.3f, -2.0f, -2.5f),
        glm::vec3(1.5f, 2.0f, -2.5f),
        glm::vec3(1.5f, 0.2f, -1.5f),
        glm::vec3(-1.3f, 1.0f, -2.5f)
    };

    //----------------------------------------------------------
    // VAO & VBO & EBO for BOX
    glGenBuffers(1, &boxVBO);

    glGenVertexArrays(1, &boxVAO);

    //unsigned int EBO;
    //glGenBuffers(1, &EBO);

    // binding
    glBindVertexArray(boxVAO);

    glBindBuffer(GL_ARRAY_BUFFER, boxVBO);
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

    container2Diffuse = Load2DTexture(rootpath + "/images/container2.png", Texture2DType::DIFFUSE, true);
    container2Specular = Load2DTexture(rootpath + "/images/container2_specular.png", Texture2DType::SPECULAR, true);
    container2Emissive = Load2DTexture(rootpath + "/images/matrix.jpg", Texture2DType::EMISSIVE, true);
    
    //----------------------------------------------------------
    // VAO & VBO & EBO for LIGHT
    glGenBuffers(1, &lightVBO);

    glGenVertexArrays(1, &lightVAO);

    //unsigned int EBO;
    //glGenBuffers(1, &EBO);

    // binding
    glBindVertexArray(lightVAO);

    glBindBuffer(GL_ARRAY_BUFFER, lightVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);

	// ----------------------------------------------------------------------
    // FRAME BUFFFER
	// framebuffer
	float quadVertices[] =
	{
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		1.0f, -1.0f,  1.0f, 0.0f,
		1.0f,  1.0f,  1.0f, 1.0f
	};

	glGenFramebuffers(1, &frameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

	glGenTextures(1, &textureColorBuffer);
	glBindTexture(GL_TEXTURE_2D, textureColorBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, (GLsizei)game->gfxContext.width, (GLsizei)game->gfxContext.height, 0, GL_RGB, GL_UNSIGNED_INT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorBuffer, 0);

	GLuint depthStencilBuffer;
	glGenRenderbuffers(1, &depthStencilBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthStencilBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, (GLsizei)game->gfxContext.width, (GLsizei)game->gfxContext.height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, depthStencilBuffer);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "ERROR::FRAMEBUFFER:: FRAMEBUFFER is not complete." << std::endl;
	}

	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &quadVBO);
	glBindVertexArray(quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

    //----------------------------------------------------------
    // SKYBOX
    glGenTextures(1, &skyboxTexture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture);

    std::vector<std::string> skyboxFaces
    {
        rootpath + "/images/skybox/right.jpg",      // +x
        rootpath + "/images/skybox/left.jpg",       // -x
        rootpath + "/images/skybox/top.jpg",        // +y
        rootpath + "/images/skybox/bottom.jpg",     // ...
        rootpath + "/images/skybox/front.jpg",
        rootpath + "/images/skybox/back.jpg"
    };

    int width, height, nrChannels;
    unsigned char* data;
    stbi_set_flip_vertically_on_load(false);
    for (size_t i = 0; i < skyboxFaces.size(); ++i)
    {
        data = stbi_load(skyboxFaces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap texture failed to load at path: " << skyboxFaces[i] << std::endl;
            stbi_image_free(data);
        }
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    float skyboxVertices[] = {
        // positions          
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f,  1.0f
    };

    GLuint skyboxVBO;

    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    //unsigned int uniformBlockIndexRed = glGetUniformBlockIndex(lightshaderProgramRed.id, "Matrices");
    //unsigned int uniformBlockIndexBlue = glGetUniformBlockIndex(lightshaderProgramBlue.id, "Matrices");
    //unsigned int uniformBlockIndexYellow = glGetUniformBlockIndex(lightshaderProgramYellow.id, "Matrices");

    //glUniformBlockBinding(lightshaderProgramRed.id, uniformBlockIndexRed, 0);
    //glUniformBlockBinding(lightshaderProgramBlue.id, uniformBlockIndexBlue, 0);
    //glUniformBlockBinding(lightshaderProgramYellow.id, uniformBlockIndexYellow, 0);

    glGenBuffers(1, &uboMatrices);
    glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
    glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    glBindBufferRange(GL_UNIFORM_BUFFER, 0, uboMatrices, 0, 2 * sizeof(glm::mat4));
    
    //glm::mat4 projection = Rain::Camera()->GetProjectionMatrix();
    glm::mat4 projection = game->camera->projectionMatrix;

    glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(projection));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    //----------------------------------------------------------
    // SHADERS
    skyboxShader.init(rootpath + "/shaders/skybox.vs", rootpath + "/shaders/skybox.fs");
    screenquadShader.init(rootpath + "/shaders/screen_quad.vs", rootpath + "/shaders/screen_quad.fs");
    shaderProgram.init(rootpath + "/shaders/shader1.vs", rootpath + "/shaders/shader1.fs");
    lightshaderProgramRed.init(rootpath + "/shaders/light_shader.vs", rootpath + "/shaders/light_shader_red.fs");
    lightshaderProgramBlue.init(rootpath + "/shaders/light_shader.vs", rootpath + "/shaders/light_shader_blue.fs");
    lightshaderProgramYellow.init(rootpath + "/shaders/light_shader.vs", rootpath + "/shaders/light_shader_yellow.fs");
	outlineShaderProgram.init(rootpath + "/shaders/shader1.vs", rootpath + "/shaders/outline.fs");


    model = LoadModel(rootpath + "/models/nanosuit/nanosuit.obj", rootpath + "/shaders/shader1.json", rootpath + "/shaders/shader1");
    //model = LoadModel(rootpath + "/models/sponza_obj/sponza.obj", rootpath + "/shaders/shader1.json", rootpath + "/shaders/shader1");


    //entity = (Entity*)malloc(sizeof(Entity));
    //bzero(entity, sizeof(Entity));
    //entity->name = "ent1";
    //Transform* t1 = GetNewHeapTransform();
    //AddComponent<Transform>(entity, t1);
    //Movement* m1 = GetNewMovement();
    //m1->direction = glm::vec3(1, 0, 0);
    //m1->speed = 3;
    //AddComponent<Movement>(entity, m1);
}



void loadShaders()
{
    skyboxShader.Reload();
    skyboxShader.use();
    skyboxShader.setParameter("skybox", 0);

    screenquadShader.Reload();
    screenquadShader.use();
    screenquadShader.setParameter("screenTexture", 0);

    for (size_t i = 0; i < model.meshes.size(); ++i)
    {
        model.meshes[i]->material.shader->use();
        model.meshes[i]->material.shader->setParameter("light1.type", 0);
        model.meshes[i]->material.shader->setParameter("light1.direction", -0.2f, -1.0f, -0.3f);
        model.meshes[i]->material.shader->setParameter("light1.ambient", 0.5f, 0.5f, 0.5f);
        model.meshes[i]->material.shader->setParameter("light1.diffuse", 0.1f, 0.1f, 0.1f);
        model.meshes[i]->material.shader->setParameter("light1.specular", 1.0f, 1.0f, 1.0f);
        model.meshes[i]->material.shader->setParameter("light2.type", 1);
        model.meshes[i]->material.shader->setParameter("light2.ambient", 0.1f, 0.1f, 0.1f);
        model.meshes[i]->material.shader->setParameter("light2.diffuse", 0.8f, 0.8f, 0.8f);
        model.meshes[i]->material.shader->setParameter("light2.specular", 1.0f, 1.0f, 1.0f);
        model.meshes[i]->material.shader->setParameter("light2.constant", 1.0f);
        model.meshes[i]->material.shader->setParameter("light2.linear", 0.09f);
        model.meshes[i]->material.shader->setParameter("light2.quadratic", 0.032f);
        model.meshes[i]->material.shader->setParameter("light3.cutOff", glm::cos(glm::radians(12.5f)));
        model.meshes[i]->material.shader->setParameter("light3.cutOff", glm::cos(glm::radians(12.5f)));
        model.meshes[i]->material.shader->setParameter("light3.outerCutOff", glm::cos(glm::radians(15.0f)));
        model.meshes[i]->material.shader->setParameter("light3.ambient", 0.1f, 0.1f, 0.1f);
        model.meshes[i]->material.shader->setParameter("light3.diffuse", 0.8f, 0.8f, 0.8f);
        model.meshes[i]->material.shader->setParameter("light3.specular", 1.0f, 1.0f, 1.0f);
        model.meshes[i]->material.shader->setParameter("light3.constant", 1.0f);
        model.meshes[i]->material.shader->setParameter("light3.linear", 0.09f);
        model.meshes[i]->material.shader->setParameter("light3.quadratic", 0.032f);
        model.meshes[i]->material.shader->setParameter("mat.diffuse", 0);
        model.meshes[i]->material.shader->setParameter("mat.specular", 1);
        model.meshes[i]->material.shader->setParameter("mat.emissive", 2);
        model.meshes[i]->material.shader->setParameter("mat.shininess", 32.0f);
        model.meshes[i]->material.shader->setParameter("skybox", 3);
    }

    shaderProgram.Reload();
    shaderProgram.use();
    //directional light
    shaderProgram.setParameter("light1.type", 0);
    shaderProgram.setParameter("light1.direction", -0.2f, -1.0f, -0.3f);
    shaderProgram.setParameter("light1.ambient", 0.5f, 0.5f, 0.5f);
    shaderProgram.setParameter("light1.diffuse", 0.1f, 0.1f, 0.1f);
    shaderProgram.setParameter("light1.specular", 1.0f, 1.0f, 1.0f);
    // point light

    shaderProgram.setParameter("light2.type", 1);
    shaderProgram.setParameter("light2.ambient", 0.1f, 0.1f, 0.1f);
    shaderProgram.setParameter("light2.diffuse", 0.8f, 0.8f, 0.8f);
    shaderProgram.setParameter("light2.specular", 1.0f, 1.0f, 1.0f);
    shaderProgram.setParameter("light2.constant", 1.0f);
    shaderProgram.setParameter("light2.linear", 0.09f);
    shaderProgram.setParameter("light2.quadratic", 0.032f);
    // spot light
    shaderProgram.setParameter("light3.cutOff", glm::cos(glm::radians(12.5f)));
    shaderProgram.setParameter("light3.cutOff", glm::cos(glm::radians(12.5f)));
    shaderProgram.setParameter("light3.outerCutOff", glm::cos(glm::radians(15.0f)));
    shaderProgram.setParameter("light3.ambient", 0.1f, 0.1f, 0.1f);
    shaderProgram.setParameter("light3.diffuse", 0.8f, 0.8f, 0.8f);
    shaderProgram.setParameter("light3.specular", 1.0f, 1.0f, 1.0f);
    shaderProgram.setParameter("light3.constant", 1.0f);
    shaderProgram.setParameter("light3.linear", 0.09f);
    shaderProgram.setParameter("light3.quadratic", 0.032f);
    // material
    shaderProgram.setParameter("mat.diffuse", 0);
    shaderProgram.setParameter("mat.specular", 1);
    shaderProgram.setParameter("mat.emissive", 2);
    shaderProgram.setParameter("mat.shininess", 32.0f);
    // env mapping
    shaderProgram.setParameter("skybox", 3);

    lightshaderProgramRed.Reload();
    lightshaderProgramBlue.Reload();
    lightshaderProgramYellow.Reload();

    outlineShaderProgram.Reload();
}

void sandboxUpdate()
{
    if (vsync)
    {
        glfwSwapInterval(1);
    }
    else
    {
        glfwSwapInterval(0);
    }
    checkInputs();

    //UpdateTransforms(entity, 1, game->deltaTime);

    // recup mat view, mat proj
    Camera* _camera = game->camera;
    Transform* camTransform = &_camera->transform;

    glm::mat4 proj = _camera->projectionMatrix;
    glm::mat4 view = GetViewMatrix(*_camera);
     //= _camera->GetViewMatrix(camTransform->position);
    glm::mat4 modelmat = glm::mat4(1);

    static int angle = 0;
    angle = (++angle) % 10;
    //Scale(modelTransform, glm::vec3(glm::sin(glfwGetTime()), glm::sin(glfwGetTime()), glm::sin(glfwGetTime())));
    //Rotate(modelTransform, glm::vec3(0.2, 0.7, 0.1), angle);


    lightPos = glm::vec3(glm::sin(glfwGetTime()) * 4, 0, glm::cos(glfwGetTime()) * 4);
    glm::mat4 lightModel = glm::mat4(1);
    lightModel = glm::translate(lightModel, lightPos);
    lightModel = glm::scale(lightModel, glm::vec3(0.5f, 0.5f, 0.5f));

    //glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_STENCIL_TEST);


    glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(view));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    lightshaderProgramRed.use();
    lightshaderProgramRed.setParameter("model", lightModel);
    glBindVertexArray(lightVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    lightModel = glm::translate(glm::mat4(1), glm::vec3(0, glm::sin(glfwGetTime()) * 4, glm::cos(glfwGetTime()) * 4));
    lightModel = glm::scale(lightModel, glm::vec3(0.5f, 0.5f, 0.5f));
    lightshaderProgramBlue.use();
    lightshaderProgramRed.setParameter("model", lightModel);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    lightModel = glm::translate(glm::mat4(1), glm::vec3(glm::cos(glfwGetTime()) * 4, glm::sin(glfwGetTime()) * 4, 0));
    lightModel = glm::scale(lightModel, glm::vec3(0.5f, 0.5f, 0.5f));
    lightshaderProgramYellow.use();
    lightshaderProgramRed.setParameter("model", lightModel);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);

    // render
    glStencilFunc(GL_ALWAYS, 1, 0xFF);
	glStencilMask(0xFF);

    for (size_t i = 0; i < model.meshes.size(); ++i)
    {
        model.meshes[i]->material.shader->use();
        model.meshes[i]->material.shader->setParameter("model", GetModelMatrix(modelTransform));
        model.meshes[i]->material.shader->setParameter("proj", proj);
        model.meshes[i]->material.shader->setParameter("view", view);
        model.meshes[i]->material.shader->setParameter("viewPos", camTransform->position);
        model.meshes[i]->material.shader->setParameter("pointLight.position", lightPos);
        model.meshes[i]->material.shader->setParameter("spotLight.position", camTransform->position);
        model.meshes[i]->material.shader->setParameter("spotLight.direction", _camera->front);
        Draw(model.meshes[i]);
    }

    shaderProgram.use();
    shaderProgram.setParameter("model", modelmat);
    shaderProgram.setParameter("proj", proj);
    shaderProgram.setParameter("view", view);
    shaderProgram.setParameter("viewPos", camTransform->position);
	shaderProgram.setParameter("pointLight.position", lightPos);
	shaderProgram.setParameter("spotLight.position", camTransform->position);
	shaderProgram.setParameter("spotLight.direction", _camera->front);
    

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, container2Diffuse.id);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, container2Specular.id);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, container2Emissive.id);
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture);

    glBindVertexArray(boxVAO);
	for (int i = 0; i < 10; ++i)
	{
		glm::mat4 model = glm::mat4(1);
		model = glm::translate(model, cubePositions[i]);
		float angle = 20.0f * i;
		model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
		shaderProgram.setParameter("model", model);

		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
	glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	glStencilMask(0x00);
	glDisable(GL_DEPTH_TEST);
	outlineShaderProgram.use();
	outlineShaderProgram.setParameter("model", modelmat);
	outlineShaderProgram.setParameter("proj", proj);
	outlineShaderProgram.setParameter("view", view);
	for (int i = 0; i < 10; ++i)
	{
		glm::mat4 model = glm::mat4(1);
		model = glm::translate(model, cubePositions[i]);
		float angle = 20.0f * i;
		model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
		model = glm::scale(model, glm::vec3(1.1f, 1.1f, 1.1f));
		outlineShaderProgram.setParameter("model", model);

		glDrawArrays(GL_TRIANGLES, 0, 36);
	}

    glBindVertexArray(0);
	glStencilMask(0xFF);
	glEnable(GL_DEPTH_TEST);

    glDepthFunc(GL_LEQUAL);
    skyboxShader.use();
    skyboxShader.setParameter("view", glm::mat4(glm::mat3(view)));
    skyboxShader.setParameter("projection", proj);

    glBindVertexArray(skyboxVAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
    glDepthFunc(GL_LESS);

    //glBindFramebuffer(GL_FRAMEBUFFER, 0);
    //glDisable(GL_DEPTH_TEST);
    //glDisable(GL_STENCIL_TEST);
    //glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    //glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    //screenquadShader.use();
    //glBindVertexArray(quadVAO);
    //glActiveTexture(GL_TEXTURE0);
    //glBindTexture(GL_TEXTURE_2D, textureColorBuffer);
    //glDrawArrays(GL_TRIANGLES, 0, 6);
}

void checkInputs()
{
    if (IsKeyPressed(game->input, GLFW_KEY_ENTER))
    //if (game->inputEngine->IsKeyPressed(GLFW_KEY_ENTER))
    {
        if (!wireframe)
        {
            std::cout << "keypressed" << std::endl;
            glfwSwapInterval(0);
            //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }
        else
        {
            glfwSwapInterval(0);
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }
        wireframe = !wireframe;
    }
    if (IsKeyPressed(game->input, GLFW_KEY_R))
    //if (game->inputEngine->IsKeyPressed(GLFW_KEY_R))
    {
        loadShaders();
    }
}