#include "pch.h"
#include "ModelDemo.h"
#include "MeshRenderer.h"
#include "Camera.h"
#include "LightRenderer.h"
#include "Model.h"
#include "SkyBoxRenderer.h"
#include "ReflectionCube.h"


ModelDemo::ModelDemo()
{
}


ModelDemo::~ModelDemo()
{
}

void ModelDemo::Initialze()
{
	glEnable(GL_DEPTH_TEST);

	//camera
	cam_ = new Camera(45.0f, 1280.f, 720.f, 0.1f, 1000.0f, { 0.0f, 0.0f, 100.0f });

	//쉐이더는 컴파일하면 여러 지오메트리에서 공유할 수 있다.
	//매번 컴파일할것없이 부모만 컴파일해서 사용하면 된다. 여기서 언리얼의 메터리얼인스턴스 개념도 출발한다.
	//Draw할때, 파라미터(uniform)에 해당하는 것만 바꾸면 된다.    
	GLuint shaderProgram = ShaderManager::GetInstance()->GetProgram("Assets/Shaders/FlatModel.vs", "Assets/Shaders/FlatModel.fs");
	assert(shaderProgram != GL_FALSE);

	light_ = new LightRenderer(MeshType::Sphere, cam_);
	light_->SetProgram(shaderProgram);
	light_->SetPosition({ 19.f, 19.f, 13.0f });

	screenShader_ = ShaderManager::GetInstance()->GetProgram("Assets/Shaders/PostProcess.vs", "Assets/Shaders/PostProcess.fs");
	modelShder_ = ShaderManager::GetInstance()->GetProgram("Assets/Shaders/meshModel.vs", "Assets/Shaders/meshModel.fs");
	meshModel_ = new Model("Assets/Objects/samplemodel/BOSS_model_final.fbx");
	//meshModel = new Model("DarkElf01.3ds");
	meshModel_->SetCamera(cam_);
	meshModel_->SetLight(light_);
	meshModel_->SetScale(glm::vec3(8.0f));
	meshModel_->SetPosition(glm::vec3(0, -4, 0));

	skybox = new SkyBoxRenderer(MeshType::Cube, cam_);
	skybox->SetName("sky");
	GLuint cubemapProgram = ShaderManager::GetInstance()->GetProgram("Assets/Shaders/cubemap.vs", "Assets/Shaders/cubemap.fs");
	skybox->SetProgram(cubemapProgram);
	GLuint skyTexture = TextureManager::GetInstance()->GetCubemapTextureID("Assets/Textures/skybox/mountain", ".tga");
	g_GLEngine->SetEnvrionmentMap(skyTexture);
	skybox->SetTexture(0, skyTexture);
	skybox->SetScale(glm::vec3(100.0f));
	skybox->SetPosition(glm::vec3(0.0, 0, 0));

	reflectionCube = new ReflectionCube(MeshType::Sphere, cam_);
	skybox->SetName("reflectionCube");
	GLuint reflectProgram = ShaderManager::GetInstance()->GetProgram("Assets/Shaders/reflection.vs", "Assets/Shaders/reflection.fs");
	reflectionCube->SetProgram(reflectProgram);
	reflectionCube->SetTexture(0, skyTexture);
	reflectionCube->SetScale(glm::vec3(10.0f));
	reflectionCube->SetPosition(glm::vec3(-4.0, 0, 0));

	refractionCube = new ReflectionCube(MeshType::Sphere, cam_);
	refractionCube->SetName("refractionCube");
	GLuint refractProgram = ShaderManager::GetInstance()->GetProgram("Assets/Shaders/reflection.vs", "Assets/Shaders/refraction.fs");
	refractionCube->SetProgram(refractProgram);
	refractionCube->SetTexture(0, skyTexture);
	refractionCube->SetScale(glm::vec3(10.0f));
	refractionCube->SetPosition(glm::vec3(-4.0, -2, 0));
}

void ModelDemo::Finalize()
{
	delete cam_;
	delete light_;
	delete debugQuad;
	delete meshModel_;

	delete skybox;
	delete reflectionCube;
	delete refractionCube;
}

void ModelDemo::UpdateScene(double deltaTimeMs)
{
	double areaFactor = 30.0f;
	double posFactorX = glm::sin(glfwGetTime()) * areaFactor;
	double posFactorY = glm::cos(glfwGetTime()) * areaFactor;

	//auto pos = light->GetPosition();
	glm::vec3 pos = { posFactorX, posFactorY, 20.0f };
	light_->SetPosition(pos);

	skybox->UpdateScene(nullptr, deltaTimeMs);

	reflectionCube->UpdateScene(nullptr, deltaTimeMs);
	refractionCube->UpdateScene(nullptr, deltaTimeMs);
}

void ModelDemo::RenderScene()
{
	Application::RenderScene();
}

void ModelDemo::ProcessKeyboard(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	
}

class Camera* ModelDemo::GetCamera()
{
	return cam_;
}

void ModelDemo::PostRenderScene()
{
	skybox->Draw();	//최적화 안해서, 먼저 그림
	meshModel_->Draw(modelShder_);


	reflectionCube->Draw();
	refractionCube->Draw();
}

void ModelDemo::PreRenderScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
