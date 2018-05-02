
#include <iostream>
#include <fstream>
#include <windows.h>
#include <string>
#include <sstream>
#include <vector>
#include <iterator>
#include <unordered_map>

#include "utility/incl_3d.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "rain.h"
#include "graphics/shader.h"
#include "core/game_engine.h"
#include "graphics/camera.h"
#include "camera_controller.h"
#include "core/transform.h"
#include "input/input_engine.h"
#include "graphics/light.h"

std::unordered_map<std::string, std::string> _args;
std::string rootpath;
bool wireframe;
rain::Shader shaderProgram;
rain::Shader lightshaderProgram;
rain::Shader outlineShaderProgram;
rain::Shader screenquadShader;
rain::Shader skyboxShader;
GLuint quadVAO, quadVBO;
GLuint frameBuffer;
GLuint textureColorBuffer;
GLuint boxVBO;
GLuint boxVAO;
GLuint lightVBO;
GLuint lightVAO;
GLuint container2Diffuse;
GLuint container2Specular;
GLuint container2Emissive;
GLuint skyboxVAO;
GLuint skyboxTexture;
glm::vec3 lightPos;
std::vector<glm::vec3> cubePositions;

void sandboxInit();
void sandboxUpdate();
void checkInputs();
GLuint loadTexture2D(const std::string& path, GLuint _internalFormat, GLenum _format, bool flipVertically = false);

int main(int argc, char** argv)
{
    for (int i = 0; i < argc; i++)
    {
        std::vector<std::string> arg = rain::Rain::split(argv[i], '=');
        if (arg.size() > 1)
        {
            _args[arg[0]] = arg[1];
        }
    }

    rain::Rain::Init(_args);
    rain::Transform* camTransform = rain::Rain::Engine()->GetCameraController()->GetTransform();
    camTransform->Translate(glm::vec3(0, 0, 5));

    rain::Rain::Engine()->SetUpdateCallback(sandboxUpdate);
    rootpath = rain::Rain::ResourcesRoot();
    wireframe = false;

	sandboxInit();
    rain::Rain::Run();

    return 0;
}

void sandboxInit()
{

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
        0, 1, 3,   // first triangle
        1, 2, 3    // second triangle
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

    container2Diffuse = loadTexture2D("/images/container2.png", GL_RGB, GL_RGBA, true);
    container2Specular = loadTexture2D("/images/container2_specular.png", GL_RGB, GL_RGBA, true);
    container2Emissive = loadTexture2D("/images/matrix.jpg", GL_RGB, GL_RGB, true);

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
	// shader
	std::string quadVertexPath = rootpath + "/shaders/screen_quad.vs";
	std::string quadFragmentPath = rootpath + "/shaders/screen_quad.fs";
	screenquadShader.init(quadVertexPath, quadFragmentPath);
	screenquadShader.use();
	screenquadShader.setParameter("screenTexture", 0);

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
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 600, 0, GL_RGB, GL_UNSIGNED_INT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorBuffer, 0);

	GLuint depthStencilBuffer;
	glGenRenderbuffers(1, &depthStencilBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthStencilBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 800, 600);
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
    for (int i = 0; i < skyboxFaces.size(); ++i)
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

    std::string skyboxVertexPath = rootpath + "/shaders/skybox.vs";
    std::string skyboxFragmentPath = rootpath + "/shaders/skybox.fs";
    skyboxShader.init(skyboxVertexPath, skyboxFragmentPath);
    skyboxShader.use();
    skyboxShader.setParameter("skybox", 0);

    //----------------------------------------------------------
    // shader
    std::string vertexPath = rootpath + "/shaders/shader1.vs";
    std::string fragmentPath = rootpath + "/shaders/shader1.fs";
    rain::Light light;
    light.Type = rain::Light::Type::DIRECTIONAL;
    light.direction = glm::vec3(-0.2f, -1.0f, -0.3f);
    light.ambient = glm::vec3(1.f, 1.f, 1.f);
    glm::vec3 lightColor = glm::vec3(1.f, 1.f, 1.f);
    light.diffuse = lightColor * glm::vec3(0.5f);
    light.ambient = light.diffuse * glm::vec3(0.1f);


    shaderProgram.init(vertexPath, fragmentPath);
    shaderProgram.use();
	//directional light
	shaderProgram.setParameter("dirLight.direction", light.direction);
	shaderProgram.setParameter("dirLight.ambient", light.diffuse);
    shaderProgram.setParameter("dirLight.diffuse", light.ambient);
    shaderProgram.setParameter("dirLight.specular", 1.0f, 1.0f, 1.0f);

	// point light
	shaderProgram.setParameter("pointLight.ambient", 0.1f, 0.1f, 0.1f);
	shaderProgram.setParameter("pointLight.diffuse", 0.8f, 0.8f, 0.8f);
	shaderProgram.setParameter("pointLight.specular", 1.0f, 1.0f, 1.0f);
	shaderProgram.setParameter("pointLight.constant", 1.0f);
	shaderProgram.setParameter("pointLight.linear", 0.09f);
	shaderProgram.setParameter("pointLight.quadratic", 0.032f);

	// spot light
	shaderProgram.setParameter("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
	shaderProgram.setParameter("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));
	shaderProgram.setParameter("spotLight.ambient", 0.1f, 0.1f, 0.1f);
	shaderProgram.setParameter("spotLight.diffuse", 0.8f, 0.8f, 0.8f);
	shaderProgram.setParameter("spotLight.specular", 1.0f, 1.0f, 1.0f);
	shaderProgram.setParameter("spotLight.constant", 1.0f);
	shaderProgram.setParameter("spotLight.linear", 0.09f);
	shaderProgram.setParameter("spotLight.quadratic", 0.032f);

	// material
    shaderProgram.setParameter("mat.diffuse", 0);
    shaderProgram.setParameter("mat.specular", 1);
    shaderProgram.setParameter("mat.emissive", 2);
    shaderProgram.setParameter("mat.shininess", 32.0f);

    // env mapping
    shaderProgram.setParameter("skybox", 3);


	// light cube shader
    std::string lightVertexPath = rootpath + "/shaders/light_shader.vs";
    std::string lightFragmentPath = rootpath + "/shaders/light_shader.fs";
    lightPos = glm::vec3(3, 1, 2);
    lightshaderProgram.init(lightVertexPath, lightFragmentPath);
    lightshaderProgram.use();
    lightshaderProgram.setParameter("color", glm::vec3(1.0f, 1.0f, 1.0f));

	// outline cube shader
	std::string outlineVextexPath = rootpath + "/shaders/shader1.vs";
	std::string outlineFragmentPath = rootpath + "/shaders/outline.fs";
	outlineShaderProgram.init(outlineVextexPath, outlineFragmentPath);
}

void sandboxUpdate()
{
    checkInputs();

    // recup mat view, mat proj
    rain::CameraController* camController = rain::Rain::Engine()->GetCameraController();
    rain::Camera* camera = camController->GetCamera();
    rain::Transform* camTransform = camController->GetTransform();

    glm::mat4 proj = camera->GetProjectionMatrix();
    glm::mat4 view = camera->GetViewMatrix(camTransform->Position);
    glm::mat4 model = glm::mat4(1);

    lightPos = glm::vec3(glm::sin(glfwGetTime()) * 4, 0, glm::cos(glfwGetTime()) * 4);
    glm::mat4 lightModel = glm::mat4(1);
    lightModel = glm::translate(lightModel, lightPos);
    lightModel = glm::scale(lightModel, glm::vec3(0.5f, 0.5f, 0.5f));



	//glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_STENCIL_TEST);

    lightshaderProgram.use();
    lightshaderProgram.setParameter("model", lightModel);
    lightshaderProgram.setParameter("proj", proj);
    lightshaderProgram.setParameter("view", view);
    glBindVertexArray(lightVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);

    

    // render
    glStencilFunc(GL_ALWAYS, 1, 0xFF);
	glStencilMask(0xFF);
    shaderProgram.use();
    shaderProgram.setParameter("model", model);
    shaderProgram.setParameter("proj", proj);
    shaderProgram.setParameter("view", view);
    shaderProgram.setParameter("viewPos", camTransform->Position);
	shaderProgram.setParameter("pointLight.position", lightPos);
	shaderProgram.setParameter("spotLight.position", camTransform->Position);
	shaderProgram.setParameter("spotLight.direction", camera->Front);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, container2Diffuse);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, container2Specular);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, container2Emissive);
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
	outlineShaderProgram.setParameter("model", model);
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
    skyboxShader.setParameter("view", glm::mat4(glm::mat3(camera->GetViewMatrix(camTransform->Position))));
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
    if (rain::Rain::Input()->IsKeyPressed(GLFW_KEY_ENTER))
    {
        if (!wireframe)
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }
        else
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }
        wireframe = !wireframe;
    }
    if (rain::Rain::Input()->IsKeyPressed(GLFW_KEY_R))
    {
        shaderProgram.reload();
        shaderProgram.use();
		rain::Light light;
		light.Type = rain::Light::Type::DIRECTIONAL;
		light.direction = glm::vec3(-0.2f, -1.0f, -0.3f);
		light.ambient = glm::vec3(1.f, 1.f, 1.f);
		glm::vec3 lightColor = glm::vec3(1.f, 1.f, 1.f);
		light.diffuse = lightColor * glm::vec3(0.5f);
		light.ambient = light.diffuse * glm::vec3(0.1f);

		//directional light
		shaderProgram.setParameter("dirLight.direction", light.direction);
		shaderProgram.setParameter("dirLight.ambient", light.diffuse);
		shaderProgram.setParameter("dirLight.diffuse", light.ambient);
		shaderProgram.setParameter("dirLight.specular", 1.0f, 1.0f, 1.0f);

		// point light
		shaderProgram.setParameter("pointLight.ambient", 0.1f, 0.1f, 0.1f);
		shaderProgram.setParameter("pointLight.diffuse", 0.8f, 0.8f, 0.8f);
		shaderProgram.setParameter("pointLight.specular", 1.0f, 1.0f, 1.0f);
		shaderProgram.setParameter("pointLight.constant", 1.0f);
		shaderProgram.setParameter("pointLight.linear", 0.09f);
		shaderProgram.setParameter("pointLight.quadratic", 0.032f);

		// material
		shaderProgram.setParameter("mat.diffuse", 0);
		shaderProgram.setParameter("mat.specular", 1);
		shaderProgram.setParameter("mat.emissive", 2);
		shaderProgram.setParameter("mat.shininess", 32.0f);
    }
}


GLuint loadTexture2D(const std::string& path, GLuint _internalFormat, GLenum _format, bool flipVertically)
{
    GLuint id;
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_set_flip_vertically_on_load(flipVertically);
    if (path != "")
    {
        int width, height, nrChannels;
        unsigned char * data = stbi_load(std::string(rootpath + path).c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, _internalFormat, width, height, 0, _format, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
        {
            std::cout << "Failed to load texture" << std::endl;
            
        }
        stbi_image_free(data);
    }
    else
    {
        int width, height;
        glTexImage2D(GL_TEXTURE_2D, 0, _internalFormat, width, height, 0, _format, GL_UNSIGNED_BYTE, NULL);
    }
    return id;
}