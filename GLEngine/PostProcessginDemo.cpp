#include "pch.h"
#include "PostProcessginDemo.h"
#include "MeshRenderer.h"
#include "Camera.h"
#include "LightRenderer.h"
#include "Mesh.h"
#include "Model.h"


PostProcessginDemo::PostProcessginDemo()
{
}


PostProcessginDemo::~PostProcessginDemo()
{
}

void PostProcessginDemo::Initialze()
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

	modelShder_ = ShaderManager::GetInstance()->GetProgram("Assets/Shaders/meshModel.vs", "Assets/Shaders/meshModel.fs");
	meshModel_ = new Model("Assets/Objects/samplemodel/BOSS_model_final.fbx");
	//meshModel = new Model("DarkElf01.3ds");
	meshModel_->SetCamera(cam_);
	meshModel_->SetLight(light_);
	meshModel_->SetScale(glm::vec3(8.0f));
	meshModel_->SetPosition(glm::vec3(0, -4, 0));

	GLuint textureShaderProgram = ShaderManager::GetInstance()->GetProgram("Assets/Shaders/TexturedModel.vs", "Assets/Shaders/TexturedModel.fs");
	assert(textureShaderProgram != GL_FALSE);
	debugQuad = new MeshRenderer(MeshType::Cube, cam_);
	debugQuad->SetProgram(textureShaderProgram);
	debugQuad->SetPosition({ -0.0f, 0.0f, 0.0f });
	debugQuad->SetScale(glm::vec3(68.0f, 40.0f, 40.0f));
}

void PostProcessginDemo::Finalize()
{
	delete cam_;
	delete light_;
	delete debugQuad;
	delete meshModel_;
}

void PostProcessginDemo::UpdateScene(double deltaTimeMs)
{
	debugQuad->UpdateScene(nullptr, deltaTimeMs);
}

void PostProcessginDemo::RenderScene()
{
	Application::RenderScene();
}

class Camera* PostProcessginDemo::GetCamera()
{
	return cam_;
}

void PostProcessginDemo::ProcessKeyboard(GLFWwindow* window, int key, int scancode, int action, int mods)
{

}

void PostProcessginDemo::PreRenderScene()
{
	glViewport(0, 0, g_GLEngine->GetScreenSizeX(), g_GLEngine->GetScreenSizeY());
	glBindFramebuffer(GL_FRAMEBUFFER, g_GLEngine->GetSceneBuffer());

	glClearColor(0.0f, 0.0f, 0.0f, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // 지금은 stencil buffer를 사용하지 않습니다
	glEnable(GL_DEPTH_TEST);
	meshModel_->PreDraw(modelShder_);

	glBindFramebuffer(GL_FRAMEBUFFER, 0); // 다시 기본값으로
}

void PostProcessginDemo::PostRenderScene()
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	//meshModel_->PostDraw(modelShder_);

	debugQuad->SetProgram(screenShader_);
	debugQuad->SetTexture(0, g_GLEngine->GetSceneTexture());
	debugQuad->Draw();
}
