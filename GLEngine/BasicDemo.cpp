#include "pch.h"
#include "BasicDemo.h"
#include "MeshRenderer.h"
#include "LitMeshRenderer.h"
#include "LightRenderer.h"
#include "Camera.h"
#include <assert.h>


BasicDemo::BasicDemo()
{
}


BasicDemo::~BasicDemo()
{
}

void BasicDemo::Initialze()
{
	glEnable(GL_DEPTH_TEST);
	cam_ = new Camera(45.0f, 1280.f, 720.f, 0.1f, 1000.0f, { 0.0f, 0.0f, 100.0f });

	//���̴��� �������ϸ� ���� ������Ʈ������ ������ �� �ִ�.
	//�Ź� �������Ұ;��� �θ� �������ؼ� ����ϸ� �ȴ�. ���⼭ �𸮾��� ���͸����ν��Ͻ� ���䵵 ����Ѵ�.
	//Draw�Ҷ�, �Ķ����(uniform)�� �ش��ϴ� �͸� �ٲٸ� �ȴ�.    
	GLuint shaderProgram = ShaderManager::GetInstance()->GetProgram("Assets/Shaders/FlatModel.vs", "Assets/Shaders/FlatModel.fs");
	assert(shaderProgram != GL_FALSE);

	light_ = new LightRenderer(MeshType::Sphere, cam_);
	light_->SetProgram(shaderProgram);
	light_->SetPosition({ 19.f, 19.f, 13.0f });


	//unlit static mesh
	GLuint textureShaderProgram = ShaderManager::GetInstance()->GetProgram("Assets/Shaders/TexturedModel.vs", "Assets/Shaders/TexturedModel.fs");
	assert(textureShaderProgram != GL_FALSE);

	//�ؽ�ó ���� ĳ���ؼ� ���������ϴ�.	
	GLuint sphereTexture = TextureManager::GetInstance()->GetTextureID("Assets/Textures/globe.dds");


	//lit static mesh
	GLuint textureLightShaderProgram = ShaderManager::GetInstance()->GetProgram("Assets/Shaders/litTexturedModel.vs", "Assets/Shaders/litTexturedModel.fs");
	assert(textureLightShaderProgram != GL_FALSE);
	litMesh_ = new LitMeshRenderer(MeshType::Sphere, cam_, light_);
	litMesh_->SetProgram(textureLightShaderProgram);

	litMesh_->SetPosition({ 16.0f, 0.0f, 0.0f });
	litMesh_->SetScale(glm::vec3(8.0f));
	litMesh_->SetTexture(0, sphereTexture);


	bottom_ = new LitMeshRenderer(MeshType::Cube, cam_, light_);
	bottom_->SetProgram(textureLightShaderProgram);
	bottom_->SetPosition({ 0.0f, -20.0f, 0.0f });
	bottom_->SetScale(glm::vec3(100.0f, 2.0f, 100.0f));
	GLuint groundTexture = TextureManager::GetInstance()->GetTextureID("Assets/Textures/ground.dds");
	bottom_->SetTexture(0, groundTexture);

	mesh_ = new MeshRenderer(MeshType::Sphere, cam_);
	mesh_->SetProgram(textureShaderProgram);
	mesh_->SetPosition({ 0.0f, 0.0f, 0.0f });
	mesh_->SetScale(glm::vec3(8.0f));
	mesh_->SetTexture(0, sphereTexture);
}

void BasicDemo::Finalize()
{
	delete mesh_;
	delete litMesh_;
	delete bottom_;
	delete cam_;
	delete light_;
}

void BasicDemo::UpdateScene(double deltaTimeMs)
{
	double areaFactor = 30.0f;
	double posFactorX = glm::sin(glfwGetTime()) * areaFactor;
	double posFactorY = glm::cos(glfwGetTime()) * areaFactor;

	//auto pos = light->GetPosition();
	glm::vec3 pos = { posFactorX, posFactorY, 20.0f };
	light_->SetPosition(pos);
}

void BasicDemo::RenderScene()
{
	Application::RenderScene();
}

void BasicDemo::ProcessKeyboard(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	
}

void BasicDemo::PostRenderScene()
{
	light_->Draw();
	mesh_->Draw();
	litMesh_->Draw();
	bottom_->Draw();
}

void BasicDemo::PreRenderScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
