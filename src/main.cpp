//#include <iostream>
//#include <fstream>
//#include <windows.h>
//#include <string>
//#include <sstream>
//#include <vector>
//#include <iterator>
//#include <unordered_map>
//#include <nlohmann/json.hpp>
//
//#include "utility/incl_3d.h"
//#include "rain.h"
//#include "graphics/shader.h"
//#include "graphics/camera.h"
//#include "core/transform.h"
//#include "input/input.h"
//#include "graphics/light.h"
//#include "utility/gl_utils.h"
//#include "graphics/material.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
//#include "graphics/model.h"
//#include "graphics/texture.h"
//#include "utility/string_utils.h"
//#include "ecs/entity.h"
//#include "ecs/system_transform.h"
//#include "core/memory.h"
//
//using namespace rain;
//
//std::string rootpath;
//bool wireframe;
//Shader shaderProgram;
//Shader lightshaderProgramRed;
//Shader lightshaderProgramBlue;
//Shader lightshaderProgramYellow;
//Shader outlineShaderProgram;
//Shader screenquadShader;
//Shader skyboxShader;
//GLuint quadVAO, quadVBO;
//GLuint frameBuffer;
//GLuint textureColorBuffer;
//GLuint boxVBO;
//GLuint boxVAO;
//GLuint lightVBO;
//GLuint lightVAO;
//GLuint uboMatrices;
//Texture2D container2Diffuse;
//Texture2D container2Specular;
//Texture2D container2Emissive;
//GLuint skyboxVAO;
//GLuint skyboxTexture;
//glm::vec3 lightPos;
//std::vector<glm::vec3> cubePositions;
//Model model;
//Game* game;
//Transform modelTransform;
//bool vsync;
//
//Entity* entity;
//
//
//void sandboxInit();
//void sandboxUpdate();
//void checkInputs();
//void loadShaders();
//
//#define RAIN_DLLEXPORT extern "C" __declspec(dllexport)
//
//RAIN_DLLEXPORT void LoadGame(const char* path)
//{
//
//
//
//
//    vsync = true;
//    game = InitGame(path);
//    //Transform* camTransform = game->camera->GetTransform();
//    Transform* camTransform = &game->camera->transform;
//    Translate(*camTransform, glm::vec3(0, 0, 5));
//
//    rootpath = game->dataPath;
//    wireframe = false;
//
//    sandboxInit();
//    loadShaders();
//
//    RunGame(game, sandboxUpdate);
//}
//
//RAIN_DLLEXPORT void OpenWindow(void* wndHandle)
//{
//    ////TCHAR* AppClassName = _T("Windows Cipher");
//    //HACCEL hAccel = 0;												// no accelerator table in this example.
//    //MSG Msg;
//    //RECT R;
//    //HWND Wnd = (HWND)wndHandle;											
//    //
//    //SetClassLongPtr(Wnd, GCL_WNDPROC, reinterpret_cast<LONG_PTR>(OpenGLMDIChildHandler));
//    //GetClientRect(GetDesktopWindow(), &R);							
//    //while (GetMessage(&Msg, 0, 0, 0))
//    //{
//    //    TranslateMessage(&Msg);									
//    //    DispatchMessage(&Msg);									
//    //};
//}
//
//RAIN_DLLEXPORT void EnableVsync()
//{
//    vsync = true;
//}
//
//RAIN_DLLEXPORT void DisableVsync()
//{
//    vsync = false;
//}
//
//
//
//int main(int argc, char** argv)
//{
//
//    /*Entity* ent = (Entity*)malloc(sizeof(ent));
//    bzero(ent, sizeof(Entity));
//    CModel* model = CreateComponent<CModel>(ent);*/
//
//    game = InitGame(argc, argv);
//    //Transform* camTransform = game->camera->GetTransform();
//    Transform* camTransform = &game->camera->transform;
//    Translate(*camTransform, glm::vec3(0, 0, 5));
//
//    rootpath = game->dataPath;
//    wireframe = false;
//
//    sandboxInit();
//    loadShaders();
//
//    RunGame(game, sandboxUpdate);
//    glfwSwapInterval(0);
//
//    return 0;
//}
//
//void sandboxInit()
//{
//    Scale(modelTransform, glm::vec3(0.3, 0.3, 0.3));
//
//    float vertices[] = {
//        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
//        0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
//        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
//        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
//        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
//        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
//
//        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
//        0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
//        0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
//        0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
//        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
//        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
//
//        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
//        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
//        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
//        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
//        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
//        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
//
//        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
//        0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
//        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
//        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
//        0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
//        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
//
//        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
//        0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
//        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
//        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
//        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
//        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
//
//        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
//        0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
//        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
//        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
//        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
//        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
//    };
//
//    GLuint indices[] =
//    {
//        0, 1, 3,
//        1, 2, 3 
//    };
//
//    cubePositions = 
//    {
//        glm::vec3(0.0f, 0.0f, 0.0f),
//        glm::vec3(2.0f, 5.0f, -15.0f),
//        glm::vec3(-1.5f, -2.2f, -2.5f),
//        glm::vec3(-3.8f, -2.0f, -12.3f),
//        glm::vec3(2.4f, -0.4f, -3.5f),
//        glm::vec3(-1.7f, 3.0f, -7.5f),
//        glm::vec3(1.3f, -2.0f, -2.5f),
//        glm::vec3(1.5f, 2.0f, -2.5f),
//        glm::vec3(1.5f, 0.2f, -1.5f),
//        glm::vec3(-1.3f, 1.0f, -2.5f)
//    };
//
//    //----------------------------------------------------------
//    // VAO & VBO & EBO for BOX
//    glGenBuffers(1, &boxVBO);
//
//    glGenVertexArrays(1, &boxVAO);
//
//    //unsigned int EBO;
//    //glGenBuffers(1, &EBO);
//
//    // binding
//    glBindVertexArray(boxVAO);
//
//    glBindBuffer(GL_ARRAY_BUFFER, boxVBO);
//    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
//
//    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
//    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
//
//    // position
//    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
//    glEnableVertexAttribArray(0);
//    // normals
//    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
//    glEnableVertexAttribArray(1);
//    // texture coord attribute
//    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
//    glEnableVertexAttribArray(2);
//
//    glBindVertexArray(0);
//
//    container2Diffuse = Load2DTexture(rootpath + "/images/container2.png", Texture2DType::DIFFUSE, true);
//    container2Specular = Load2DTexture(rootpath + "/images/container2_specular.png", Texture2DType::SPECULAR, true);
//    container2Emissive = Load2DTexture(rootpath + "/images/matrix.jpg", Texture2DType::EMISSIVE, true);
//    
//    //----------------------------------------------------------
//    // VAO & VBO & EBO for LIGHT
//    glGenBuffers(1, &lightVBO);
//
//    glGenVertexArrays(1, &lightVAO);
//
//    //unsigned int EBO;
//    //glGenBuffers(1, &EBO);
//
//    // binding
//    glBindVertexArray(lightVAO);
//
//    glBindBuffer(GL_ARRAY_BUFFER, lightVBO);
//    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
//
//    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
//    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
//
//    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
//    glEnableVertexAttribArray(0);
//
//    glBindVertexArray(0);
//
//	// ----------------------------------------------------------------------
//    // FRAME BUFFFER
//	// framebuffer
//	float quadVertices[] =
//	{
//		-1.0f,  1.0f,  0.0f, 1.0f,
//		-1.0f, -1.0f,  0.0f, 0.0f,
//		1.0f, -1.0f,  1.0f, 0.0f,
//
//		-1.0f,  1.0f,  0.0f, 1.0f,
//		1.0f, -1.0f,  1.0f, 0.0f,
//		1.0f,  1.0f,  1.0f, 1.0f
//	};
//
//	glGenFramebuffers(1, &frameBuffer);
//	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
//
//	glGenTextures(1, &textureColorBuffer);
//	glBindTexture(GL_TEXTURE_2D, textureColorBuffer);
//    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, (GLsizei)game->gfxContext.width, (GLsizei)game->gfxContext.height, 0, GL_RGB, GL_UNSIGNED_INT, NULL);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorBuffer, 0);
//
//	GLuint depthStencilBuffer;
//	glGenRenderbuffers(1, &depthStencilBuffer);
//	glBindRenderbuffer(GL_RENDERBUFFER, depthStencilBuffer);
//	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, (GLsizei)game->gfxContext.width, (GLsizei)game->gfxContext.height);
//	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, depthStencilBuffer);
//
//	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
//	{
//		std::cout << "ERROR::FRAMEBUFFER:: FRAMEBUFFER is not complete." << std::endl;
//	}
//
//	glGenVertexArrays(1, &quadVAO);
//	glGenBuffers(1, &quadVBO);
//	glBindVertexArray(quadVAO);
//	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
//	glEnableVertexAttribArray(0);
//	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
//	glEnableVertexAttribArray(1);
//	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
//
//	glBindFramebuffer(GL_FRAMEBUFFER, 0);
//
//    //----------------------------------------------------------
//    // SKYBOX
//    glGenTextures(1, &skyboxTexture);
//    glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture);
//
//    std::vector<std::string> skyboxFaces
//    {
//        rootpath + "/images/skybox/right.jpg",      // +x
//        rootpath + "/images/skybox/left.jpg",       // -x
//        rootpath + "/images/skybox/top.jpg",        // +y
//        rootpath + "/images/skybox/bottom.jpg",     // ...
//        rootpath + "/images/skybox/front.jpg",
//        rootpath + "/images/skybox/back.jpg"
//    };
//
//    int width, height, nrChannels;
//    unsigned char* data;
//    stbi_set_flip_vertically_on_load(false);
//    for (size_t i = 0; i < skyboxFaces.size(); ++i)
//    {
//        data = stbi_load(skyboxFaces[i].c_str(), &width, &height, &nrChannels, 0);
//        if (data)
//        {
//            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
//            stbi_image_free(data);
//        }
//        else
//        {
//            std::cout << "Cubemap texture failed to load at path: " << skyboxFaces[i] << std::endl;
//            stbi_image_free(data);
//        }
//    }
//
//    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
//
//    float skyboxVertices[] = {
//        // positions          
//		-1.0f,  1.0f, -1.0f,
//		-1.0f, -1.0f, -1.0f,
//		1.0f, -1.0f, -1.0f,
//		1.0f, -1.0f, -1.0f,
//		1.0f,  1.0f, -1.0f,
//		-1.0f,  1.0f, -1.0f,
//
//		-1.0f, -1.0f,  1.0f,
//		-1.0f, -1.0f, -1.0f,
//		-1.0f,  1.0f, -1.0f,
//		-1.0f,  1.0f, -1.0f,
//		-1.0f,  1.0f,  1.0f,
//		-1.0f, -1.0f,  1.0f,
//
//		1.0f, -1.0f, -1.0f,
//		1.0f, -1.0f,  1.0f,
//		1.0f,  1.0f,  1.0f,
//		1.0f,  1.0f,  1.0f,
//		1.0f,  1.0f, -1.0f,
//		1.0f, -1.0f, -1.0f,
//
//		-1.0f, -1.0f,  1.0f,
//		-1.0f,  1.0f,  1.0f,
//		1.0f,  1.0f,  1.0f,
//		1.0f,  1.0f,  1.0f,
//		1.0f, -1.0f,  1.0f,
//		-1.0f, -1.0f,  1.0f,
//
//		-1.0f,  1.0f, -1.0f,
//		1.0f,  1.0f, -1.0f,
//		1.0f,  1.0f,  1.0f,
//		1.0f,  1.0f,  1.0f,
//		-1.0f,  1.0f,  1.0f,
//		-1.0f,  1.0f, -1.0f,
//
//		-1.0f, -1.0f, -1.0f,
//		-1.0f, -1.0f,  1.0f,
//		1.0f, -1.0f, -1.0f,
//		1.0f, -1.0f, -1.0f,
//		-1.0f, -1.0f,  1.0f,
//		1.0f, -1.0f,  1.0f
//    };
//
//    GLuint skyboxVBO;
//
//    glGenVertexArrays(1, &skyboxVAO);
//    glGenBuffers(1, &skyboxVBO);
//    glBindVertexArray(skyboxVAO);
//    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
//    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
//    glEnableVertexAttribArray(0);
//    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
//
//    //unsigned int uniformBlockIndexRed = glGetUniformBlockIndex(lightshaderProgramRed.id, "Matrices");
//    //unsigned int uniformBlockIndexBlue = glGetUniformBlockIndex(lightshaderProgramBlue.id, "Matrices");
//    //unsigned int uniformBlockIndexYellow = glGetUniformBlockIndex(lightshaderProgramYellow.id, "Matrices");
//
//    //glUniformBlockBinding(lightshaderProgramRed.id, uniformBlockIndexRed, 0);
//    //glUniformBlockBinding(lightshaderProgramBlue.id, uniformBlockIndexBlue, 0);
//    //glUniformBlockBinding(lightshaderProgramYellow.id, uniformBlockIndexYellow, 0);
//
//    glGenBuffers(1, &uboMatrices);
//    glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
//    glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
//    glBindBuffer(GL_UNIFORM_BUFFER, 0);
//
//    glBindBufferRange(GL_UNIFORM_BUFFER, 0, uboMatrices, 0, 2 * sizeof(glm::mat4));
//    
//    //glm::mat4 projection = Rain::Camera()->GetProjectionMatrix();
//    glm::mat4 projection = game->camera->projectionMatrix;
//
//    glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
//    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(projection));
//    glBindBuffer(GL_UNIFORM_BUFFER, 0);
//
//    //----------------------------------------------------------
//    // SHADERS
//    skyboxShader.init(rootpath + "/shaders/skybox.vs", rootpath + "/shaders/skybox.fs");
//    screenquadShader.init(rootpath + "/shaders/screen_quad.vs", rootpath + "/shaders/screen_quad.fs");
//    shaderProgram.init(rootpath + "/shaders/shader1.vs", rootpath + "/shaders/shader1.fs");
//    lightshaderProgramRed.init(rootpath + "/shaders/light_shader.vs", rootpath + "/shaders/light_shader_red.fs");
//    lightshaderProgramBlue.init(rootpath + "/shaders/light_shader.vs", rootpath + "/shaders/light_shader_blue.fs");
//    lightshaderProgramYellow.init(rootpath + "/shaders/light_shader.vs", rootpath + "/shaders/light_shader_yellow.fs");
//	outlineShaderProgram.init(rootpath + "/shaders/shader1.vs", rootpath + "/shaders/outline.fs");
//
//
//    //model = LoadModel(rootpath + "/models/nanosuit/nanosuit.obj", rootpath + "/shaders/shader1.json", rootpath + "/shaders/shader1");
//    model = LoadModel(rootpath + "/models/sponza_obj/sponza.obj", rootpath + "/shaders/shader1.json", rootpath + "/shaders/shader1");
//
//    //Entity* t1 = new Entity();
//    //CTransform* ctrans = CreateComponent<CTransform>(t1);
//    //Translate(ctrans->transform, glm::vec3(5, 5, 5));
//    //CModel* cmodel = new CModel();
//    //AddComponent<CModel>(t1, cmodel);
//
//    entity = (Entity*)malloc(sizeof(Entity));
//    bzero(entity, sizeof(Entity));
//    entity->name = "ent1";
//    Transform* t1 = GetNewHeapTransform();
//    AddComponent<Transform>(entity, t1);
//    Movement* m1 = GetNewMovement();
//    m1->direction = glm::vec3(1, 0, 0);
//    m1->speed = 3;
//    AddComponent<Movement>(entity, m1);
//}
//
//
//
//void loadShaders()
//{
//    skyboxShader.Reload();
//    skyboxShader.use();
//    skyboxShader.setParameter("skybox", 0);
//
//    screenquadShader.Reload();
//    screenquadShader.use();
//    screenquadShader.setParameter("screenTexture", 0);
//
//    for (size_t i = 0; i < model.meshes.size(); ++i)
//    {
//        model.meshes[i]->material.shader->use();
//        model.meshes[i]->material.shader->setParameter("light1.type", 0);
//        model.meshes[i]->material.shader->setParameter("light1.direction", -0.2f, -1.0f, -0.3f);
//        model.meshes[i]->material.shader->setParameter("light1.ambient", 0.5f, 0.5f, 0.5f);
//        model.meshes[i]->material.shader->setParameter("light1.diffuse", 0.1f, 0.1f, 0.1f);
//        model.meshes[i]->material.shader->setParameter("light1.specular", 1.0f, 1.0f, 1.0f);
//        model.meshes[i]->material.shader->setParameter("light2.type", 1);
//        model.meshes[i]->material.shader->setParameter("light2.ambient", 0.1f, 0.1f, 0.1f);
//        model.meshes[i]->material.shader->setParameter("light2.diffuse", 0.8f, 0.8f, 0.8f);
//        model.meshes[i]->material.shader->setParameter("light2.specular", 1.0f, 1.0f, 1.0f);
//        model.meshes[i]->material.shader->setParameter("light2.constant", 1.0f);
//        model.meshes[i]->material.shader->setParameter("light2.linear", 0.09f);
//        model.meshes[i]->material.shader->setParameter("light2.quadratic", 0.032f);
//        model.meshes[i]->material.shader->setParameter("light3.cutOff", glm::cos(glm::radians(12.5f)));
//        model.meshes[i]->material.shader->setParameter("light3.cutOff", glm::cos(glm::radians(12.5f)));
//        model.meshes[i]->material.shader->setParameter("light3.outerCutOff", glm::cos(glm::radians(15.0f)));
//        model.meshes[i]->material.shader->setParameter("light3.ambient", 0.1f, 0.1f, 0.1f);
//        model.meshes[i]->material.shader->setParameter("light3.diffuse", 0.8f, 0.8f, 0.8f);
//        model.meshes[i]->material.shader->setParameter("light3.specular", 1.0f, 1.0f, 1.0f);
//        model.meshes[i]->material.shader->setParameter("light3.constant", 1.0f);
//        model.meshes[i]->material.shader->setParameter("light3.linear", 0.09f);
//        model.meshes[i]->material.shader->setParameter("light3.quadratic", 0.032f);
//        model.meshes[i]->material.shader->setParameter("mat.diffuse", 0);
//        model.meshes[i]->material.shader->setParameter("mat.specular", 1);
//        model.meshes[i]->material.shader->setParameter("mat.emissive", 2);
//        model.meshes[i]->material.shader->setParameter("mat.shininess", 32.0f);
//        model.meshes[i]->material.shader->setParameter("skybox", 3);
//    }
//
//    shaderProgram.Reload();
//    shaderProgram.use();
//    //directional light
//    shaderProgram.setParameter("light1.type", 0);
//    shaderProgram.setParameter("light1.direction", -0.2f, -1.0f, -0.3f);
//    shaderProgram.setParameter("light1.ambient", 0.5f, 0.5f, 0.5f);
//    shaderProgram.setParameter("light1.diffuse", 0.1f, 0.1f, 0.1f);
//    shaderProgram.setParameter("light1.specular", 1.0f, 1.0f, 1.0f);
//    // point light
//
//    shaderProgram.setParameter("light2.type", 1);
//    shaderProgram.setParameter("light2.ambient", 0.1f, 0.1f, 0.1f);
//    shaderProgram.setParameter("light2.diffuse", 0.8f, 0.8f, 0.8f);
//    shaderProgram.setParameter("light2.specular", 1.0f, 1.0f, 1.0f);
//    shaderProgram.setParameter("light2.constant", 1.0f);
//    shaderProgram.setParameter("light2.linear", 0.09f);
//    shaderProgram.setParameter("light2.quadratic", 0.032f);
//    // spot light
//    shaderProgram.setParameter("light3.cutOff", glm::cos(glm::radians(12.5f)));
//    shaderProgram.setParameter("light3.cutOff", glm::cos(glm::radians(12.5f)));
//    shaderProgram.setParameter("light3.outerCutOff", glm::cos(glm::radians(15.0f)));
//    shaderProgram.setParameter("light3.ambient", 0.1f, 0.1f, 0.1f);
//    shaderProgram.setParameter("light3.diffuse", 0.8f, 0.8f, 0.8f);
//    shaderProgram.setParameter("light3.specular", 1.0f, 1.0f, 1.0f);
//    shaderProgram.setParameter("light3.constant", 1.0f);
//    shaderProgram.setParameter("light3.linear", 0.09f);
//    shaderProgram.setParameter("light3.quadratic", 0.032f);
//    // material
//    shaderProgram.setParameter("mat.diffuse", 0);
//    shaderProgram.setParameter("mat.specular", 1);
//    shaderProgram.setParameter("mat.emissive", 2);
//    shaderProgram.setParameter("mat.shininess", 32.0f);
//    // env mapping
//    shaderProgram.setParameter("skybox", 3);
//
//    lightshaderProgramRed.Reload();
//    lightshaderProgramBlue.Reload();
//    lightshaderProgramYellow.Reload();
//
//    outlineShaderProgram.Reload();
//}
//
//void sandboxUpdate()
//{
//    if (vsync)
//    {
//        glfwSwapInterval(1);
//    }
//    else
//    {
//        glfwSwapInterval(0);
//    }
//    checkInputs();
//
//    UpdateTransforms(entity, 1, game->deltaTime);
//
//    // recup mat view, mat proj
//    Camera* _camera = game->camera;
//    Transform* camTransform = &_camera->transform;
//
//    glm::mat4 proj = _camera->projectionMatrix;
//    glm::mat4 view = GetViewMatrix(*_camera);
//     //= _camera->GetViewMatrix(camTransform->position);
//    glm::mat4 modelmat = glm::mat4(1);
//
//    static int angle = 0;
//    angle = (++angle) % 10;
//    //Scale(modelTransform, glm::vec3(glm::sin(glfwGetTime()), glm::sin(glfwGetTime()), glm::sin(glfwGetTime())));
//    //Rotate(modelTransform, glm::vec3(0.2, 0.7, 0.1), angle);
//
//
//    lightPos = glm::vec3(glm::sin(glfwGetTime()) * 4, 0, glm::cos(glfwGetTime()) * 4);
//    glm::mat4 lightModel = glm::mat4(1);
//    lightModel = glm::translate(lightModel, lightPos);
//    lightModel = glm::scale(lightModel, glm::vec3(0.5f, 0.5f, 0.5f));
//
//    //glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
//	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
//	glEnable(GL_DEPTH_TEST);
//	glEnable(GL_STENCIL_TEST);
//
//
//    glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
//    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(view));
//    glBindBuffer(GL_UNIFORM_BUFFER, 0);
//
//    lightshaderProgramRed.use();
//    lightshaderProgramRed.setParameter("model", lightModel);
//    glBindVertexArray(lightVAO);
//    glDrawArrays(GL_TRIANGLES, 0, 36);
//
//    lightModel = glm::translate(glm::mat4(1), glm::vec3(0, glm::sin(glfwGetTime()) * 4, glm::cos(glfwGetTime()) * 4));
//    lightModel = glm::scale(lightModel, glm::vec3(0.5f, 0.5f, 0.5f));
//    lightshaderProgramBlue.use();
//    lightshaderProgramRed.setParameter("model", lightModel);
//    glDrawArrays(GL_TRIANGLES, 0, 36);
//
//    lightModel = glm::translate(glm::mat4(1), glm::vec3(glm::cos(glfwGetTime()) * 4, glm::sin(glfwGetTime()) * 4, 0));
//    lightModel = glm::scale(lightModel, glm::vec3(0.5f, 0.5f, 0.5f));
//    lightshaderProgramYellow.use();
//    lightshaderProgramRed.setParameter("model", lightModel);
//    glDrawArrays(GL_TRIANGLES, 0, 36);
//    glBindVertexArray(0);
//
//    // render
//    glStencilFunc(GL_ALWAYS, 1, 0xFF);
//	glStencilMask(0xFF);
//
//    for (size_t i = 0; i < model.meshes.size(); ++i)
//    {
//        model.meshes[i]->material.shader->use();
//        model.meshes[i]->material.shader->setParameter("model", GetModelMatrix(modelTransform));
//        model.meshes[i]->material.shader->setParameter("proj", proj);
//        model.meshes[i]->material.shader->setParameter("view", view);
//        model.meshes[i]->material.shader->setParameter("viewPos", camTransform->position);
//        model.meshes[i]->material.shader->setParameter("pointLight.position", lightPos);
//        model.meshes[i]->material.shader->setParameter("spotLight.position", camTransform->position);
//        model.meshes[i]->material.shader->setParameter("spotLight.direction", _camera->front);
//        Draw(model.meshes[i]);
//    }
//
//    shaderProgram.use();
//    shaderProgram.setParameter("model", modelmat);
//    shaderProgram.setParameter("proj", proj);
//    shaderProgram.setParameter("view", view);
//    shaderProgram.setParameter("viewPos", camTransform->position);
//	shaderProgram.setParameter("pointLight.position", lightPos);
//	shaderProgram.setParameter("spotLight.position", camTransform->position);
//	shaderProgram.setParameter("spotLight.direction", _camera->front);
//    
//
//    glActiveTexture(GL_TEXTURE0);
//    glBindTexture(GL_TEXTURE_2D, container2Diffuse.id);
//    glActiveTexture(GL_TEXTURE1);
//    glBindTexture(GL_TEXTURE_2D, container2Specular.id);
//    glActiveTexture(GL_TEXTURE2);
//    glBindTexture(GL_TEXTURE_2D, container2Emissive.id);
//    glActiveTexture(GL_TEXTURE3);
//    glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture);
//
//    glBindVertexArray(boxVAO);
//	for (int i = 0; i < 10; ++i)
//	{
//		glm::mat4 model = glm::mat4(1);
//		model = glm::translate(model, cubePositions[i]);
//		float angle = 20.0f * i;
//		model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
//		shaderProgram.setParameter("model", model);
//
//		glDrawArrays(GL_TRIANGLES, 0, 36);
//	}
//	glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
//	glStencilMask(0x00);
//	glDisable(GL_DEPTH_TEST);
//	outlineShaderProgram.use();
//	outlineShaderProgram.setParameter("model", modelmat);
//	outlineShaderProgram.setParameter("proj", proj);
//	outlineShaderProgram.setParameter("view", view);
//	for (int i = 0; i < 10; ++i)
//	{
//		glm::mat4 model = glm::mat4(1);
//		model = glm::translate(model, cubePositions[i]);
//		float angle = 20.0f * i;
//		model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
//		model = glm::scale(model, glm::vec3(1.1f, 1.1f, 1.1f));
//		outlineShaderProgram.setParameter("model", model);
//
//		glDrawArrays(GL_TRIANGLES, 0, 36);
//	}
//
//    glBindVertexArray(0);
//	glStencilMask(0xFF);
//	glEnable(GL_DEPTH_TEST);
//
//    glDepthFunc(GL_LEQUAL);
//    skyboxShader.use();
//    skyboxShader.setParameter("view", glm::mat4(glm::mat3(view)));
//    skyboxShader.setParameter("projection", proj);
//
//    glBindVertexArray(skyboxVAO);
//    glActiveTexture(GL_TEXTURE0);
//    glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture);
//    glDrawArrays(GL_TRIANGLES, 0, 36);
//    glBindVertexArray(0);
//    glDepthFunc(GL_LESS);
//
//    //glBindFramebuffer(GL_FRAMEBUFFER, 0);
//    //glDisable(GL_DEPTH_TEST);
//    //glDisable(GL_STENCIL_TEST);
//    //glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
//    //glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
//
//    //screenquadShader.use();
//    //glBindVertexArray(quadVAO);
//    //glActiveTexture(GL_TEXTURE0);
//    //glBindTexture(GL_TEXTURE_2D, textureColorBuffer);
//    //glDrawArrays(GL_TRIANGLES, 0, 6);
//}
//
//void checkInputs()
//{
//    if (IsKeyPressed(game->input, GLFW_KEY_ENTER))
//    //if (game->inputEngine->IsKeyPressed(GLFW_KEY_ENTER))
//    {
//        if (!wireframe)
//        {
//            std::cout << "keypressed" << std::endl;
//            glfwSwapInterval(1);
//            //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
//        }
//        else
//        {
//            //glfwSwapInterval(1);
//            //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
//        }
//        wireframe = !wireframe;
//    }
//    if (IsKeyPressed(game->input, GLFW_KEY_R))
//    //if (game->inputEngine->IsKeyPressed(GLFW_KEY_R))
//    {
//        loadShaders();
//    }
//}




#include <vector>
#include <iostream>
#include <stdint.h>
#include <ctime>
#include <time.h>
#include <stdlib.h>

#define bzero(b,len) (memset((b), '\0', (len)), (void) 0)

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

void PrintBit(size_t const size, void const * const ptr)
{
	unsigned char *b = (unsigned char*)ptr;
	unsigned char byte;
	int i, j;

	for (i = size - 1; i >= 0; i--)
	{
		for (j = 7; j >= 0; j--)
		{
			byte = (b[i] >> j) & 1;
			printf("%u", byte);
		}
	}
	puts("");
}


//------------------------------------------
struct vec3
{
	float x;
	float y;
	float z;
};

vec3 operator*(const vec3& _vec, const u32& _float)
{
	vec3 vec;
	vec.x = _vec.x * _float;
	vec.y = _vec.y * _float;
	vec.z = _vec.z * _float;
	return vec;
}

vec3 operator+(const vec3& _vec, const u32& _float)
{
	vec3 vec;
	vec.x = _vec.x + _float;
	vec.y = _vec.y + _float;
	vec.z = _vec.z + _float;
	return vec;
}

vec3 operator+(const vec3& _vec1, const vec3& _vec2)
{
	vec3 vec;
	vec.x = _vec1.x + _vec2.x;
	vec.y = _vec1.y + _vec2.y;
	vec.z = _vec1.z + _vec2.z;
	return vec;
}

vec3 operator+=(const vec3& _vec1, const vec3& _vec2)
{
	vec3 vec;
	vec.x = _vec1.x + _vec2.x;
	vec.y = _vec1.y + _vec2.y;
	vec.z = _vec1.z + _vec2.z;
	return vec;
}

struct GLSLVariable
{
	unsigned int id;
	char* name;
	u32 size;
};

//------------------------------------------

struct Component
{
	// entity uses a bitmask to register which component it contains
	// this enum doesnt represent bitflags
	// it is used to shift bits the right number of times to make a bitmask -- i think that should work
	enum class Type : u32
	{
		TRANSFORM = 0,
		SCRIPT,
		PHYSICS,
		MODEL,
		MAX_COUNT
	};

	Component::Type type;
};

struct Transform : public Component
{
	vec3 position;
	vec3 rotation;
	vec3 scale;
};

Component* CreateTransform()
{
	Transform* transform = (Transform*)malloc(sizeof(Transform));
	transform->type = Component::Type::TRANSFORM;
	transform->position = { 0, 0, 0 };
	transform->rotation = { 0, 0, 0 };
	transform->scale = { 1, 1, 1 };
	return transform;
}

void PrintTransform(Transform* _transform)
{
	std::cout << std::endl << "(" << _transform->position.x << "," << _transform->position.y << "," << _transform->position.z << ")" << std::endl;
}

struct Script : Component
{
	void(*Init)(void);
	void(*Update)(void);
	void(*Shutdown)(void);
};

Component* CreateScript(void(*Init)(void), void(*Update)(void), void(*Shutdown)(void))
{
	Script* script = (Script*)malloc(sizeof(Script));
	script->type = Component::Type::SCRIPT;
	script->Init = Init;
	script->Update = Update;
	script->Shutdown = Shutdown;
	return script;
}

struct Physics : Component
{
	float weight;
	float friction;
	bool grounded;
	vec3 direction;
	float speed;
};

Component* CreatePhysics()
{
	Physics* physics = (Physics*)malloc(sizeof(Physics));
	physics->type = Component::Type::PHYSICS;
	physics->friction = 0;
	physics->grounded = false;
	physics->weight = 0;
	physics->direction = { 0, 0, 0 };
	physics->speed = 0;
	return physics;
}

struct Model : Component
{
	vec3* vertices;
	vec3* indices;
	u32 size;
};

Component* CreateModel()
{
	Model* model = (Model*)malloc(sizeof(Model));
	model->type = Component::Type::MODEL;
	model->indices = nullptr;
	model->vertices = nullptr;
	model->size = 0;
	return model;
}


//------------------------------------

// TODO(martin): add stepcount
struct Entity
{
	u64 id;
	char* name;
	u64 flags;
	Component** components;
	u32 size;
	u32 capacity;
	u32 sizeStep;
};

Entity* CreateEntity(u64 _id, const char* _name, u32 _capacity = 0, u32 _sizeStep = 10)
{
	Entity* entity = (Entity*)calloc(1, sizeof(Entity));
	entity->id = _id;
	entity->sizeStep = _sizeStep;

	entity->name = (char*)malloc((sizeof(_name) + 1));
	strcpy_s(entity->name, sizeof(entity->name) + 1, _name);
	entity->capacity = _capacity;
	if (entity->capacity > 0)
	{
		entity->components = (Component**)calloc(entity->capacity, sizeof(Component*));
	}

	return entity;
}

void AddComponent(Entity* _entity, Component* _component)
{
	u64 flag = (1 << (u64)_component->type);
	if ((_entity->flags & flag) == 0)
	{
		_entity->flags |= flag;

		if (_entity->size == _entity->capacity)
		{
			bool wasEmpty = _entity->capacity == 0;
			_entity->capacity += _entity->sizeStep;
			if (wasEmpty)
			{
				_entity->components = (Component**)calloc(_entity->capacity, sizeof(Component*));
			}
			else
			{
				Component** temp = (Component**)realloc(_entity->components, _entity->capacity * sizeof(Component*));
				_entity->components = temp;
			}
		}

		_entity->components[_entity->size] = _component;
		_entity->size++;
	}
}

void RemoveComponent(Entity* _entity, Component::Type _type)
{
	u64 flag = (1 << (u64)_type);
	if ((_entity->flags & flag) != 0)
	{
		Component** end = _entity->components + _entity->size;
		Component** iter = _entity->components;

		while (iter < end)
		{
			if ((*iter)->type == _type)
			{
				free(*iter);
				*iter = *(_entity->components + (_entity->size - 1));
				_entity->components[_entity->size - 1] = NULL;
				_entity->size--;
				return;
			}
			iter++;
		}

	}
}

Component* FindComponent(Entity* _entity, Component::Type _type)
{
	u64 flag = (1 << (u32)_type);
	if ((_entity->flags & flag) != 0)
	{
		Component** end = _entity->components + _entity->size;
		Component** iter = _entity->components;

		while (iter < end)
		{
			if ((*iter)->type == _type)
			{
				return *iter;
			}
			iter++;
		}
	}

	return nullptr;
}

bool FitsRequirements(Entity* _entity, u64 _bitmask)
{
	if ((_entity->flags & _bitmask) == _bitmask)
	{
		return true;
	}
	return false;
}


//----------------------------------------------------

struct EntityContainer
{
	Entity** entities;
	u32 size;
	u32 capacity;
	u32 resizeStep;
};

EntityContainer* CreateEntityContainer(u32 _capacity, u32 _resizeStep = 10, u32 _size = 0)
{
	EntityContainer* container = (EntityContainer*)malloc(sizeof(EntityContainer));
	container->size = 0;
	container->resizeStep = _resizeStep;
	container->capacity = _capacity;

	if (container->capacity > 0)
	{
		container->entities = (Entity**)calloc(container->capacity, sizeof(Entity*));
	}

	if (_size > 0)
	{
		Entity* entities = (Entity*)calloc(_size, sizeof(Entity));

		for (u32 i = 0; i < _size; ++i)
		{
			entities[container->size].id = container->size;
			container->entities[container->size] = &entities[container->size];
			container->size++;
		}
	}

	return container;
}

//EntityContainer* CreateEntityContainer(u32 _size, u32 _resizeStep = 10)
//{
//	EntityContainer* container = (EntityContainer*)malloc(sizeof(EntityContainer));
//	container->size = 0;
//	container->resizeStep = _resizeStep;
//	container->capacity = _size + _resizeStep;
//
//	if (container->capacity > 0)
//	{
//		container->entities = (Entity**)calloc(container->capacity, sizeof(Entity*));
//	}
//
//	if (_size > 0)
//	{
//		Entity* entities = (Entity*)calloc(_size, sizeof(Entity));
//		for (u32 i = 0; i < _size; ++i)
//		{
//			container->entities[container->size] = entities + container->size;
//			container->size++;
//		}
//	}
//
//	return container;
//}

void AddEntity(EntityContainer* _container, Entity* _entity)
{
	if (_container->size == _container->capacity)
	{
		bool wasEmpty = (_container->capacity == 0);
		_container->capacity += _container->resizeStep;
		if (wasEmpty)
		{
			_container->entities = (Entity**)calloc(_container->capacity, sizeof(Entity*));
		}
		else
		{
			Entity** temp = (Entity**)realloc(_container->entities, _container->capacity * sizeof(Entity*));
			_container->entities = temp;
		}
	}

	_container->entities[_container->size] = _entity;
	_container->size++;
}


void RemoveEntity(EntityContainer* _container, Entity* _ent)
{
	Entity** end = _container->entities + _container->size;
	Entity** iter = _container->entities;

	while (iter < end)
	{
		if ((*iter)->id == _ent->id)
		{
			free(*iter);
			*iter = *(_container->entities + (_container->size - 1));
			_container->entities[_container->size - 1] = NULL;
			_container->size--;
			return;
		}
		iter++;
	}
}


struct System
{
	u64 requirements;
	u32 size;
	u32 capacity;
	u32 resizeStep;
	Entity** entities;
};

System* CreateSystem(u64 _requirements, u32 _capacity = 0, u32 _resizeStep = 10)
{
	System* system = (System*)malloc(sizeof(System));
	system->requirements = _requirements;
	system->size = 0;
	system->capacity = _capacity;
	system->resizeStep = _resizeStep;

	if (system->capacity > 0)
	{
		system->entities = (Entity**)calloc(system->capacity, sizeof(Entity*));
	}
	return system;
}

void CheckForAddEntity(System* _system, Entity* _entity)
{
	if (FitsRequirements(_entity, _system->requirements))
	{
		if (_system->size == _system->capacity)
		{
			bool wasEmpty = (_system->capacity == 0);
			_system->capacity += _system->resizeStep;
			if (wasEmpty)
			{
				_system->entities = (Entity**)calloc(_system->capacity, sizeof(Entity*));
			}
			else
			{
				Entity** temp = (Entity**)realloc(_system->entities, _system->capacity * sizeof(Entity*));
				_system->entities = temp;
			}
		}

		_system->entities[_system->size] = _entity;
		_system->size++;
	}
}

void RemoveEntity(System* _system, Entity* _entity)
{
	Entity** end = _system->entities + _system->size;
	Entity** iter = _system->entities;

	while (iter < end)
	{
		if ((*iter)->id == _entity->id)
		{
			free(*iter);
			*iter = *(_system->entities + (_system->size - 1));
			_system->entities[_system->size - 1] = NULL;
			_system->size--;
			return;
		}
		iter++;
	}
}

void CheckForAddEntities(System* _system, Entity** _entities, u32 _size)
{
	u32 resize = _system->size;
	for (u32 i = 0; i < _size; ++i)
	{
		if (FitsRequirements(_entities[i], _system->requirements))
		{
			resize++;
		}
	}

	bool wasEmpty = (_system->capacity == 0);
	while (resize >= _system->capacity)
	{
		_system->capacity += _system->resizeStep;
	}
	if (wasEmpty)
	{
		_system->entities = (Entity**)calloc(_system->capacity, sizeof(Entity*));
	}
	else
	{
		Entity** temp = (Entity**)realloc(_system->entities, _system->capacity * sizeof(Entity*));
		_system->entities = temp;
	}
	for (u32 i = 0; i < _size; ++i)
	{
		if (FitsRequirements(_entities[i], _system->requirements))
		{
			_system->entities[_system->size] = _entities[i];
			_system->size++;
		}
	}
}

void SystemPhysicsUpdate(System* _system)
{
	for (u32 i = 0; i < _system->size; ++i)
	{
		Physics* physics = (Physics*)FindComponent(_system->entities[i], Component::Type::PHYSICS);
		Transform* transform = (Transform*)FindComponent(_system->entities[i], Component::Type::TRANSFORM);

		transform->position = transform->position + ((physics->direction * physics->speed) + (physics->friction));

		PrintTransform(transform);
	}
}



void EntityTests();
void EntityContainerTest();
void SystemTest();

int main(void)
{


	
	system("PAUSE"); return 0;
}







struct LinearMemoryAllocator
{
	void* memory;
	u32 size;
};

struct StackMemoryAllocator
{

};

struct PoolMemoryAllocator
{

};

void EntityTests()
{
	// ENTITY TEST CREATION
	Entity* ent1 = CreateEntity(1, "ent1", 0);
	Entity* ent2 = CreateEntity(2, "ent2", 1);

	// ENTITY TEST ADD COMPONENT
	Component* comp1 = (Transform*)calloc(1, sizeof(Transform));
	comp1->type = Component::Type::TRANSFORM;
	Transform* comp2 = (Transform*)malloc(sizeof(Transform));
	comp2->type = Component::Type::TRANSFORM;
	comp2->scale = { 1, 1, 1 };
	comp2->position = { 0, 0, 0 };
	comp2->rotation = { 0, 90, 0 };
	AddComponent(ent1, comp1);
	AddComponent(ent2, comp2);

	std::cout << "Entity creation : "
		<< std::endl << "-> id : " << ent1->id
		<< std::endl << "-> name : " << ent1->name
		<< std::endl << "-> flags : " << std::endl;
	PrintBit(sizeof(ent1->flags), &ent1->flags);
	std::cout << "-> components ? : " << (ent1->components != nullptr)
		<< std::endl << "-> size : " << ent1->size
		<< std::endl << "-> capacity : " << ent1->capacity
		<< std::endl << std::endl;

	std::cout << "Entity creation : "
		<< std::endl << "-> id : " << ent2->id
		<< std::endl << "-> name : " << ent2->name
		<< std::endl << "-> flags : " << std::endl;
	PrintBit(sizeof(ent2->flags), &ent2->flags);
	std::cout << "-> components ? : " << (ent2->components != nullptr)
		<< std::endl << "-> size : " << ent2->size
		<< std::endl << "-> capacity : " << ent2->capacity
		<< std::endl << std::endl;




	// ENTITY TEST FIND COMPONENT
	Transform* tran1 = (Transform*)FindComponent(ent1, Component::Type::TRANSFORM);
	if (tran1)
	{
		std::cout << std::endl << "found transform in ent1 : "
			<< std::endl << "(" << tran1->position.x << "," << tran1->position.y << "," << tran1->position.z << ")"
			<< std::endl << "(" << tran1->scale.x << "," << tran1->scale.y << "," << tran1->scale.z << ")"
			<< std::endl << "(" << tran1->rotation.x << "," << tran1->rotation.y << "," << tran1->rotation.z << ")";
	}
	else
	{
		std::cout << std::endl << "ERROR :: FINDCOMPONENT transform in ent1 failed" << std::endl;
	}

	Transform* tran2 = (Transform*)FindComponent(ent2, Component::Type::TRANSFORM);
	if (tran2)
	{
		std::cout << std::endl << "found transform in ent2 : "
			<< std::endl << "(" << tran2->position.x << "," << tran2->position.y << "," << tran2->position.z << ")"
			<< std::endl << "(" << tran2->scale.x << "," << tran2->scale.y << "," << tran2->scale.z << ")"
			<< std::endl << "(" << tran2->rotation.x << "," << tran2->rotation.y << "," << tran2->rotation.z << ")";
	}
	else
	{
		std::cout << std::endl << "ERROR :: FINDCOMPONENT transform in ent2 failed" << std::endl;
	}

	// ENTITY TEST REMOVE COMPONENT
	Physics* comp3 = (Physics*)calloc(1, sizeof(Physics));
	comp3->type = Component::Type::PHYSICS;
	comp3->grounded = true;
	comp3->weight = 127;
	comp3->friction = 42;

	AddComponent(ent1, comp3);
	Physics* phy1 = (Physics*)FindComponent(ent1, Component::Type::PHYSICS);
	if (phy1)
	{
		std::cout << std::endl << "found physics in ent1 : "
			<< std::endl << "friction : " << phy1->friction << ", grounded : " << (phy1->grounded ? "true" : "false") << ", weight :" << phy1->weight << std::endl;
	}
	else
	{
		std::cout << std::endl << "ERROR :: FINDCOMPONENT physics in ent1 failed" << std::endl;
	}
	RemoveComponent(ent1, Component::Type::PHYSICS);
	Physics* phy2 = (Physics*)FindComponent(ent1, Component::Type::PHYSICS);
	if (phy2)
	{
		std::cout << std::endl << "ERROR :: physics still in ent1" << std::endl;
	}
	else
	{
		std::cout << std::endl << "removed worked ! " << std::endl;
	}

	// ENTITY TEST FITS REQUIREMENTS
	comp3 = (Physics*)calloc(1, sizeof(Physics));
	comp3->type = Component::Type::PHYSICS;
	comp3->grounded = true;
	comp3->weight = 127;
	comp3->friction = 42;
	AddComponent(ent1, comp3);
	u64 mask1 = (1 << (u64)Component::Type::TRANSFORM) | (1 << (u64)Component::Type::PHYSICS);
	u64 mask2 = (1 << (u64)Component::Type::TRANSFORM) | (1 << (u64)Component::Type::MODEL);
	u64 mask3 = (1 << (u64)Component::Type::TRANSFORM) | (1 << (u64)Component::Type::PHYSICS) | (1 << (u64)Component::Type::MODEL);
	u64 mask4 = (1 << (u64)Component::Type::TRANSFORM);
	u64 mask5 = 0;

	bool fits1 = FitsRequirements(ent1, mask1);
	bool fits2 = FitsRequirements(ent1, mask2);
	bool fits3 = FitsRequirements(ent1, mask3);
	bool fits4 = FitsRequirements(ent1, mask4);
	bool fits5 = FitsRequirements(ent1, mask5);

	std::cout << std::endl << "1 (true) : " << (fits1 ? "true" : "false")
		<< std::endl << "2 (false) : " << (fits2 ? "true" : "false")
		<< std::endl << "3 (false) : " << (fits3 ? "true" : "false")
		<< std::endl << "4 (true) : " << (fits4 ? "true" : "false")
		<< std::endl << "5 (true) : " << (fits5 ? "true" : "false") << std::endl;

	std::cout << std::endl;
}

void EntityContainerTest()
{
	// CONTAINER CREATION
	EntityContainer* container = CreateEntityContainer(10, 7);
	// ADDING ENTITIES
	std::vector<Entity*> tempvec;

	for (u32 i = 0; i < 20; i++)
	{
		Entity* ent = CreateEntity(i, "ent");
		tempvec.push_back(ent);
		AddEntity(container, ent);
	}

	std::cout << std::endl << "after add : " << container->size << "," << container->capacity << std::endl;

	// REMOVING ENTITIES
	for (u32 i = 0; i < 18;)
	{
		RemoveEntity(container, tempvec[i]);
		i += 2;
	}
	std::cout << std::endl << "after remove : " << container->size << "," << container->capacity << std::endl;
}

void SystemTest()
{
	srand(time(NULL));
	// CONTAINER CREATION
	EntityContainer* container1 = CreateEntityContainer(10000000, 100, 10000000);
	for (int i = 0; i < 10; i++)
	{
		AddComponent(container1->entities[i], CreateTransform());
		AddComponent(container1->entities[i], CreatePhysics());
		Physics* physics = (Physics*)FindComponent(container1->entities[i], Component::Type::PHYSICS);
		if (physics)
		{
			int r = rand() % 100;
			physics->weight = rand() % 100;
			physics->friction = rand() % 200 + 100;
			physics->grounded = (rand() % 2) ? true : false;
			physics->direction = { (float)(rand() % 10), (float)(rand() % 10), (float)(rand() % 10) };
			physics->speed = rand() % 5;
		}
	}

	// SYSTEM CREATION
	u64 requirements = (1 << (u32)Component::Type::PHYSICS);
	System* system1 = CreateSystem(requirements, 100, 100);

	// SYSTEM ADD
	CheckForAddEntities(system1, container1->entities, container1->size);

	// SYSTEM UPDATE
	SystemPhysicsUpdate(system1);
}
//std::clock_t start;
//double duration;
//
//start = std::clock();
//duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;
//
//std::cout << "duration 2: " << duration << '\n';