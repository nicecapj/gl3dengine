#include "pch.h"
#include "MineCraftDemo.h"
#include "MeshRenderer.h"
#include "Camera.h"
#include "LightRenderer.h"
#include "CubemanRenderer.h"
#include "SkyBoxRenderer.h"

#include "LitMeshRenderer.h"

MineCraftDemo::MineCraftDemo()
{
}


MineCraftDemo::~MineCraftDemo()
{
}

void MineCraftDemo::Initialze()
{
	glEnable(GL_DEPTH_TEST);
	cam_ = new Camera(45.0f, 1280.f, 720.f, 0.1f, 1000.0f, { 0.0f, 0.0f, 100.0f });

	GLuint shaderProgram = ShaderManager::GetInstance()->GetProgram("Assets/Shaders/FlatModel.vs", "Assets/Shaders/FlatModel.fs");
	assert(shaderProgram != GL_FALSE);
	light_ = new LightRenderer(MeshType::Sphere, cam_);
	light_->SetProgram(shaderProgram);
	light_->SetPosition({ 19.f, 19.f, 13.0f });

	cubeman = new CubemanRenderer(cam_, light_);
	cubeman->SetName("Steve");
	GLuint steveTex = TextureManager::GetInstance()->GetTextureID("Assets/Textures/steve.jpg");
	cubeman->SetTexture(0, steveTex);
	cubeman->SetScale(glm::vec3(4.0f));
	cubeman->SetPosition(glm::vec3(16, 0, 0));
	//cubeman->SetEnableDynamicShadow(true);	

	cubeman2 = new CubemanRenderer(cam_, light_);
	cubeman2->SetName("Widow");
	GLuint widowTex = TextureManager::GetInstance()->GetTextureID("Assets/Textures/blackwidow.png");
	cubeman2->SetTexture(0, widowTex);
	cubeman2->SetScale(glm::vec3(4.0f));
	cubeman2->SetPosition(glm::vec3(8.0, 0, 0));

	skybox = new SkyBoxRenderer(MeshType::Cube, cam_);
	skybox->SetName("sky");
	GLuint cubemapProgram = ShaderManager::GetInstance()->GetProgram("Assets/Shaders/cubemap.vs", "Assets/Shaders/cubemap.fs");
	skybox->SetProgram(cubemapProgram);
	GLuint skyTexture = TextureManager::GetInstance()->GetCubemapTextureID("Assets/Textures/skybox/mountain", ".tga");
	g_GLEngine->SetEnvrionmentMap(skyTexture);
	skybox->SetTexture(0, skyTexture);
	skybox->SetScale(glm::vec3(100.0f));
	skybox->SetPosition(glm::vec3(0.0, 0, 0));	
}

void MineCraftDemo::Finalize()
{
	delete cubeman;
	delete cubeman2;
	delete skybox;
	
	delete cam_;
	delete light_;
}

void MineCraftDemo::UpdateScene(double deltaTimeMs)
{
	deltaTimeMs_ = deltaTimeMs;

	skybox->UpdateScene(nullptr, deltaTimeMs);
	cubeman->UpdateScene(nullptr, deltaTimeMs);
	cubeman2->UpdateScene(nullptr, deltaTimeMs);
}

void MineCraftDemo::RenderScene()
{
	Application::RenderScene();
}

void MineCraftDemo::ProcessKeyboard(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
	{
		glm::vec3 euler = cubeman->GetRotationEuler();
		euler += glm::vec3(0.0f, 10.0 * deltaTimeMs_, 0.0f);
		cubeman->SetRotation(euler);
	}
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
	{
		glm::vec3 euler = cubeman->GetRotationEuler();
		euler -= glm::vec3(0.0f, 10.0 * deltaTimeMs_, 0.0f);
		cubeman->SetRotation(euler);
	}
	if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
	{
		glm::vec3 euler = cubeman->GetRotationEuler();
		euler -= glm::vec3(10.0f * deltaTimeMs_, 0.0, 0.0f);
		cubeman->SetRotation(euler);
	}
	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
	{
		glm::vec3 euler = cubeman->GetRotationEuler();
		euler += glm::vec3(10.0f * deltaTimeMs_, 0.0, 0.0f);
		cubeman->SetRotation(euler);
	}
}

class Camera* MineCraftDemo::GetCamera()
{
	return cam_;
}

void MineCraftDemo::PostRenderScene()
{
	skybox->Draw();	//최적화 안해서, 먼저 그림

	cubeman->Draw();
	cubeman2->Draw();	
}

void MineCraftDemo::PreRenderScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
