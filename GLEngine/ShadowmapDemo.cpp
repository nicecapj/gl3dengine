#include "pch.h"
#include "ShadowmapDemo.h"
#include "MeshRenderer.h"
#include "Camera.h"
#include "LightRenderer.h"

GLuint depthTextureShader;
GLuint shadowmapTextureLitShader;

ShadowmapDemo::ShadowmapDemo()
{	
}


ShadowmapDemo::~ShadowmapDemo()
{	
}

void ShadowmapDemo::Initialze()
{
	glEnable(GL_DEPTH_TEST);

	//camera
	cam_ = new Camera(45.0f, 1280.f, 720.f, 0.1f, 1000.0f, { 0.0f, 5.0f, 40.0f });

	//���̴��� �������ϸ� ���� ������Ʈ������ ������ �� �ִ�.
	//�Ź� �������Ұ;��� �θ� �������ؼ� ����ϸ� �ȴ�. ���⼭ �𸮾��� ���͸����ν��Ͻ� ���䵵 ����Ѵ�.
	//Draw�Ҷ�, �Ķ����(uniform)�� �ش��ϴ� �͸� �ٲٸ� �ȴ�.    
	GLuint shaderProgram = ShaderManager::GetInstance()->GetProgram("Assets/Shaders/FlatModel.vs", "Assets/Shaders/FlatModel.fs");
	assert(shaderProgram != GL_FALSE);

	light_ = new LightRenderer(MeshType::Sphere, cam_);
	light_->SetProgram(shaderProgram);
	light_->SetPosition({ 10.f, 10.f, 10.0f });

	//texture
	sphereTexture = TextureManager::GetInstance()->GetTextureID("Assets/Textures/globe.dds");
	GLuint groundTexture = TextureManager::GetInstance()->GetTextureID("Assets/Textures/ground.dds");


	//shadowmap	
	depthTextureShader = ShaderManager::GetInstance()->GetProgram("Assets/Shaders/depthTexture.vs", "Assets/Shaders/depthTexture.fs");
	shadowmapTextureLitShader = ShaderManager::GetInstance()->GetProgram("Assets/Shaders/shadowmapTextureShader.vs", "Assets/Shaders/shadowmapTextureShader.fs");

	for (int i = 0; i < 10; ++i)
	{
		auto depthMesh = new LitMeshShadowRenderer(MeshType::Sphere, cam_, light_);
		depthMesh->SetProgram(depthTextureShader);
		depthMesh->SetPosition({ 0.0f + (4 * i), 0.0f, 0.0f });
		depthMesh->SetScale(glm::vec3(2.0f));
		//depthMesh->SetTexture(0, groundTexture);
		depthMesh->SetEnableCastDynamicShadow(true);

		shadowRenderList_.push_back(depthMesh);
	}

	auto shadowBottom = new LitMeshShadowRenderer(MeshType::Cube, cam_, light_);
	shadowBottom->SetProgram(depthTextureShader);
	shadowBottom->SetPosition({ 0.0f, -10.0f, 0.0f });
	shadowBottom->SetScale(glm::vec3(100.0f, 2.0f, 100.0f));
	shadowBottom->SetTexture(0, groundTexture);
	shadowBottom->SetEnableCastDynamicShadow(true);
	shadowRenderList_.push_back(shadowBottom);

	GLuint textureShaderProgram = ShaderManager::GetInstance()->GetProgram("Assets/Shaders/TexturedModel.vs", "Assets/Shaders/TexturedModel.fs");
	assert(textureShaderProgram != GL_FALSE);
	debugQuad = new MeshRenderer(MeshType::Cube, cam_);
	debugQuad->SetProgram(textureShaderProgram);
	debugQuad->SetPosition({ -1.0f, 1.0f, 0.0f });
	debugQuad->SetScale(glm::vec3(8.0f, 8.0f, 8.0f));
}

void ShadowmapDemo::Finalize()
{
	for (auto* renderer : shadowRenderList_)
	{
		delete renderer;
		renderer = nullptr;
	}
	shadowRenderList_.clear();

	delete cam_;
	delete light_;
	delete debugQuad;
}

void ShadowmapDemo::UpdateScene(double deltaTimeMs)
{
	double areaFactor = 30.0f;
	double posFactorX = glm::sin(glfwGetTime()) * areaFactor;
	double posFactorY = glm::cos(glfwGetTime()) * areaFactor;

	//auto pos = light_->GetPosition();
	//glm::vec3 pos = { posFactorX, posFactorY, 20.0f };
	//light_->SetPosition(pos);
	light_->UpdateScene(nullptr, deltaTimeMs);

	for (auto* renderer : shadowRenderList_)
	{
		renderer->UpdateScene(nullptr, deltaTimeMs);
	}

	debugQuad->UpdateScene(nullptr, deltaTimeMs);
}

void ShadowmapDemo::RenderScene()
{
	Application::RenderScene();	
}

class Camera* ShadowmapDemo::GetCamera()
{
	return cam_;
}

//render scene.	//������� �׸��ڰ� �߰��Ǹ� 2�� �׷��� ��. ��ο��� ���ϰ� 2��
//���� �ؽ��Ŀ� ��ü ������Ʈ 1�� ��ο�, ���� 1�� ��ο�
void ShadowmapDemo::PreRenderScene()
{
	// 1. first render to depth map
	glViewport(0, 0, g_GLEngine->GetShadowmapWidth(), g_GLEngine->GetShadowmapHeight());
	glBindFramebuffer(GL_FRAMEBUFFER, g_GLEngine->GetShadowmapBuffer());
	glClear(GL_DEPTH_BUFFER_BIT);

	for (auto renderObj : shadowRenderList_)
	{
		renderObj->SetProgram(depthTextureShader);
		renderObj->PreDraw();
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);	
}

void ShadowmapDemo::PostRenderScene()
{
	// 2. then render scene as normal with shadow mapping (using depth map)
	//ConfigureShaderAndMatrices();		
	//glBindTexture(GL_TEXTURE_2D, depthMap);
	glViewport(0, 0, g_GLEngine->GetScreenSizeX(), g_GLEngine->GetScreenSizeY());
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0, 0.0, 0.0, 1.0);

	for (auto renderObj : shadowRenderList_)
	{
		renderObj->SetProgram(shadowmapTextureLitShader);
		renderObj->SetTexture(0, sphereTexture);
		renderObj->SetTexture(1, g_GLEngine->GetShadowmap());
		renderObj->PostDraw();
	}
	
	debugQuad->SetTexture(0, g_GLEngine->GetShadowmap());
	debugQuad->Draw();	

	light_->Draw();
}
