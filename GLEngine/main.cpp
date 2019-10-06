// GLEngine.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include "pch.h"
#include <iostream>
#include <GL/eglew.h>	//opengl extention wrangler
#include <GLFW/glfw3.h>	//OpenGL, window and input
#include <gl/glew.h>

#include "GLEngine.h"
#include "Mesh.h"
#include "LightRenderer.h"
#include "MeshRenderer.h"
#include "LitMeshRenderer.h"
#include "LitInstanceMeshRenderer.h"
#include "LitMeshShadowRenderer.h"
#include "CubemanRenderer.h"
#include "Camera.h"
#include "ShaderManager.h"
#include "TextureManager.h"
#include "TextRenderer.h"
#include "SkyBoxRenderer.h"
#include "ReflectionCube.h"

#include "Model.h"

#include "Renderer.h"
#include <map>
#include <vector>
#include <string>
#include <assert.h>

const int COUNT_X = 10;
const int COUNT_Y = 10;
const float DistanceWithObject = 16.0f;
size_t amount = COUNT_Y * COUNT_X;

double deltaTime = 0;
bool isEnableWireFrame = false;
bool useInstancing = false;
bool useShadowmap = false;
bool useOrthProjection = false;
bool useHierachySample = false;
bool usePostProcessing = true;

Camera* cam = nullptr;
LightRenderer* light = nullptr;
MeshRenderer* mesh = nullptr;
LitMeshRenderer* litMesh = nullptr;
LitMeshRenderer* bottom = nullptr;
TextRenderer* label = nullptr;
//LitMeshShadowRenderer* depthMesh = nullptr;
MeshRenderer* debugQuad = nullptr;
CubemanRenderer* cubeman = nullptr;
CubemanRenderer* cubeman2 = nullptr;
SkyBoxRenderer* skybox = nullptr;
ReflectionCube* reflectionCube = nullptr;
ReflectionCube* refractionCube = nullptr;

Model* meshModel = nullptr;
Model* meshModel2 = nullptr;
Model* meshModel3 = nullptr;

GLuint modelShder = -1;
GLuint screenShader = -1;


LitInstanceMeshRenderer* instancingMesh = nullptr;
GLuint textureLightShaderProgram;
GLuint depthTextureShader;
GLuint shadowmapTextureLitShader;


GLuint instancingBuffer = -1;

std::vector<Renderer*> renderList_;
std::vector<Renderer*> shadowRenderList_;
//std::map<Renderer*, int> instancingRendererMap_;


GLuint sphereTexture = -1;

void InitSceneForInstancing(GLuint texture);
void ProcessKeyboard(GLFWwindow* window, int key, int scancode, int action, int mods);
void ProcessMouseMove(GLFWwindow* window, double xpos, double ypos);
void ProcessMouseButton(GLFWwindow* window, int button, int action, int mods);


void PreRenderScene()
{
	if (useShadowmap)
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

	if (usePostProcessing)
	{
		glViewport(0, 0, g_GLEngine->GetScreenSizeX(), g_GLEngine->GetScreenSizeY());
		glBindFramebuffer(GL_FRAMEBUFFER, g_GLEngine->GetSceneBuffer());
		
		glClearColor(0.0f, 0.0f, 0.0f, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // 지금은 stencil buffer를 사용하지 않습니다
		glEnable(GL_DEPTH_TEST);
		meshModel->PreDraw(modelShder);
		meshModel2->PreDraw(modelShder);
		meshModel3->PreDraw(modelShder);

		glBindFramebuffer(GL_FRAMEBUFFER, 0); // 다시 기본값으로
	}
}

void PostRenderScene()
{
	if (useShadowmap)
	{
		// 2. then render scene as normal with shadow mapping (using depth map)
		glViewport(0, 0, g_GLEngine->GetScreenSizeX(), g_GLEngine->GetScreenSizeY());
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.0, 0.0, 0.0, 1.0);
	}
	
	if (usePostProcessing)
	{
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//meshModel->PostDraw(modelShder);
		//meshModel2->PostDraw(modelShder);
		//meshModel3->PostDraw(modelShder);

		debugQuad->SetProgram(screenShader);
		debugQuad->SetTexture(0, g_GLEngine->GetSceneTexture());
		debugQuad->Draw();
	}
	else
	{
		meshModel->Draw(modelShder);
		meshModel2->Draw(modelShder);
		meshModel3->Draw(modelShder);
	}

	if (useHierachySample)
	{
		skybox->Draw();	//최적화 안해서, 먼저 그림
		reflectionCube->Draw();
		refractionCube->Draw();
		//cubeman->Draw(textureLightShaderProgram);		
		//cubeman2->Draw();
		light->Draw();
		label->Draw();
	
		return;
	}

	if (useShadowmap)
	{
		// 2. then render scene as normal with shadow mapping (using depth map)
		//ConfigureShaderAndMatrices();		
		//glBindTexture(GL_TEXTURE_2D, depthMap);

		for (auto renderObj : shadowRenderList_)
		{			
			renderObj->SetProgram(shadowmapTextureLitShader);
			renderObj->SetTexture(0, sphereTexture);
			renderObj->SetTexture(1, g_GLEngine->GetShadowmap());
			renderObj->PostDraw();
		}

		debugQuad->SetTexture(0, g_GLEngine->GetShadowmap());
		debugQuad->Draw();
	}
	else
	{
		if (!useInstancing)
		{
			for (auto* renderer : renderList_)
			{
				renderer->Draw();
			}
		}
		else
		{
			instancingMesh->Draw();
		}
		
		mesh->Draw();

		
		litMesh->Draw();
	}

	bottom->Draw();
	light->Draw();
	label->Draw();
}


void UpdateScene(double deltaTimeMs)
{
	if (useOrthProjection)
	{
		cam->SetOrthProjection(-100, 100, -100, 100, -10, 1000);
	}
	else
	{
		cam->SetPerspectiveProjection(45.0f, 1280.f, 720.f, 0.1f, 1000.0f);
	}

	int counter = 0;
	for (auto* renderer : renderList_)
	{
		double posFactorX = glm::sin(glfwGetTime() + (counter * deltaTimeMs));
		double posFactorY = glm::cos(glfwGetTime() + (counter * deltaTimeMs));

		auto pos = renderer->GetPosition();
		pos = { pos.x, pos.y, pos.z + posFactorX };
		renderer->SetPosition(pos);
		++counter;
	}

	for (auto* renderer : shadowRenderList_)
	{
		renderer->UpdateScene(nullptr, deltaTimeMs);
	}
	debugQuad->UpdateScene(nullptr, deltaTimeMs);

	if (useInstancing)
	{
		instancingMesh->UpdateScene(nullptr, deltaTimeMs);
	}

	if (useHierachySample)
	{
		cubeman->UpdateScene(nullptr, deltaTimeMs);
		cubeman2->UpdateScene(nullptr, deltaTimeMs);
	}

	skybox->UpdateScene(nullptr, deltaTimeMs);

	reflectionCube->UpdateScene(nullptr, deltaTimeMs);
	refractionCube->UpdateScene(nullptr, deltaTimeMs);

	{
		double areaFactor = 30.0f;
		double posFactorX = glm::sin(glfwGetTime()) * areaFactor;
		double posFactorY = glm::cos(glfwGetTime()) * areaFactor;

		//auto pos = light->GetPosition();
		glm::vec3 pos = { posFactorX, posFactorY, 20.0f };
		light->SetPosition(pos);
	}
}

void InitScene()
{
	glEnable(GL_DEPTH_TEST);
	cam = new Camera(45.0f, 1280.f, 720.f, 0.1f, 1000.0f, { 0.0f, 0.0f, 100.0f });

	//쉐이더는 컴파일하면 여러 지오메트리에서 공유할 수 있다.
	//매번 컴파일할것없이 부모만 컴파일해서 사용하면 된다. 여기서 언리얼의 메터리얼인스턴스 개념도 출발한다.
	//Draw할때, 파라미터(uniform)에 해당하는 것만 바꾸면 된다.    
	GLuint shaderProgram = ShaderManager::GetInstance()->GetProgram("Assets/Shaders/FlatModel.vs", "Assets/Shaders/FlatModel.fs");
	assert(shaderProgram != GL_FALSE);

	light = new LightRenderer(MeshType::Sphere, cam);
	light->SetProgram(shaderProgram);
	light->SetPosition({ 19.f, 19.f, 13.0f });


	//unlit static mesh
	GLuint textureShaderProgram = ShaderManager::GetInstance()->GetProgram("Assets/Shaders/TexturedModel.vs", "Assets/Shaders/TexturedModel.fs");
	assert(textureShaderProgram != GL_FALSE);

	//텍스처 역시 캐시해서 공유가능하다.
	//GLuint sphereTexture = textureLoader.GetTextureID("Assets/Textures/globe.dds");
	sphereTexture = TextureManager::GetInstance()->GetTextureID("Assets/Textures/globe.dds");

	mesh = new MeshRenderer(MeshType::Sphere, cam);
	mesh->SetProgram(textureShaderProgram);
	mesh->SetPosition({ 0.0f, 0.0f, 0.0f });
	mesh->SetScale(glm::vec3(8.0f));
	mesh->SetTexture(0, sphereTexture);


	//dynamic text
	GLuint textProgram = ShaderManager::GetInstance()->GetProgram("Assets/Shaders/text.vs", "Assets/Shaders/text.fs");
	assert(textProgram != GL_FALSE);
	label = new TextRenderer("Text", "Assets/fonts/DMSerifDisplay-Regular.ttf", 64, { 0.0f, 0.0f, 1.0f }, textProgram);
	label->SetProgram(textProgram);
	label->SetPosition(glm::vec2(320.0f, 500.0f));


	//lit static mesh
	textureLightShaderProgram = ShaderManager::GetInstance()->GetProgram("Assets/Shaders/litTexturedModel.vs", "Assets/Shaders/litTexturedModel.fs");
	assert(textureLightShaderProgram != GL_FALSE);
	litMesh = new LitMeshRenderer(MeshType::Sphere, cam, light);
	litMesh->SetProgram(textureLightShaderProgram);

	litMesh->SetPosition({ 16.0f, 0.0f, 0.0f });
	litMesh->SetScale(glm::vec3(8.0f));
	litMesh->SetTexture(0, sphereTexture);


	bottom = new LitMeshRenderer(MeshType::Cube, cam, light);
	bottom->SetProgram(textureLightShaderProgram);
	bottom->SetPosition({ 0.0f, -20.0f, 0.0f });
	bottom->SetScale(glm::vec3(100.0f, 2.0f, 100.0f));
	GLuint groundTexture = TextureManager::GetInstance()->GetTextureID("Assets/Textures/ground.dds");
	bottom->SetTexture(0, groundTexture);


	//shadowmap	
	depthTextureShader = ShaderManager::GetInstance()->GetProgram("Assets/Shaders/depthTexture.vs", "Assets/Shaders/depthTexture.fs");
	shadowmapTextureLitShader = ShaderManager::GetInstance()->GetProgram("Assets/Shaders/shadowmapTextureShader.vs", "Assets/Shaders/shadowmapTextureShader.fs");

	for (int i = 0; i < 10; ++i)
	{
		auto depthMesh = new LitMeshShadowRenderer(MeshType::Sphere, cam, light);
		depthMesh->SetProgram(depthTextureShader);
		depthMesh->SetPosition({ 32.0f + (16 * i), 0.0f, 0.0f });
		depthMesh->SetScale(glm::vec3(8.0f));
		//depthMesh->SetTexture(0, depthTextureShader);
		depthMesh->SetEnableDynamicShadow(true);

		shadowRenderList_.push_back(depthMesh);
	}

	auto shadowBottom = new LitMeshShadowRenderer(MeshType::Cube, cam, light);
	shadowBottom->SetProgram(depthTextureShader);
	shadowBottom->SetPosition({ 0.0f, -20.0f, 0.0f });
	shadowBottom->SetScale(glm::vec3(100.0f, 2.0f, 100.0f));
	shadowBottom->SetTexture(0, groundTexture);
	shadowBottom->SetEnableDynamicShadow(true);
	shadowRenderList_.push_back(shadowBottom);

	GLuint depthTextureDebugShaderProgram = ShaderManager::GetInstance()->GetProgram("Assets/Shaders/depthTextureDebug.vs", "Assets/Shaders/depthTextureDebug.fs");
	debugQuad = new MeshRenderer(MeshType::Cube, cam);
	debugQuad->SetProgram(textureShaderProgram);
	debugQuad->SetPosition({ -0.0f, 0.0f, 0.0f });
	debugQuad->SetScale(glm::vec3(68.0f, 40.0f, 40.0f));
	//debugQuad->SetTexture(sphereTexture);


	glm::vec3 basePos{ 0.0f, 0.0f, 0.0f };
	int posModFactor = COUNT_X * static_cast<int>((DistanceWithObject * 0.5f));
	for (int y = 0; y < COUNT_Y; ++y)
	{
		for (int x = 0; x < COUNT_X; ++x)
		{
			auto renderer = new LitMeshRenderer(MeshType::Sphere, cam, light);

			renderer->SetProgram(textureLightShaderProgram);	//컴파일된 쉐이더 공유

			//위치나, 회전, 스케일링은 자유롭게 바꿀수 있다.
			renderer->SetPosition(glm::vec3(basePos.x + (x * DistanceWithObject) - posModFactor, basePos.y + (y * DistanceWithObject) - posModFactor, basePos.z));
			renderer->SetScale(glm::vec3(8.0f));

			//텍스쳐도 미리 로딩해놓고 공유할수 있다. 다만 draw할때 gpu로 전송은 된다.
			renderer->SetTexture(0, sphereTexture);

			renderList_.push_back(std::move(renderer));
		}
	}

	InitSceneForInstancing(sphereTexture);

	cubeman = new CubemanRenderer(cam, light);
	cubeman->SetName("Steve");
	GLuint steveTex = TextureManager::GetInstance()->GetTextureID("Assets/Textures/steve.jpg");	
	cubeman->SetTexture(0, steveTex);
	cubeman->SetScale(glm::vec3(4.0f));
	cubeman->SetPosition(glm::vec3(16, 0, 0));
	//cubeman->SetEnableDynamicShadow(true);	

	cubeman2 = new CubemanRenderer(cam, light);
	cubeman2->SetName("Widow");
	GLuint widowTex = TextureManager::GetInstance()->GetTextureID("Assets/Textures/blackwidow.png");	
	cubeman2->SetTexture(0, widowTex);
	cubeman2->SetScale(glm::vec3(4.0f));
	cubeman2->SetPosition(glm::vec3(8.0, 0, 0));

	skybox = new SkyBoxRenderer(MeshType::Cube, cam);
	skybox->SetName("sky");
	GLuint cubemapProgram = ShaderManager::GetInstance()->GetProgram("Assets/Shaders/cubemap.vs", "Assets/Shaders/cubemap.fs");
	skybox->SetProgram(cubemapProgram);
	GLuint skyTexture = TextureManager::GetInstance()->GetCubemapTextureID("Assets/Textures/skybox/mountain", ".tga");		
	g_GLEngine->SetEnvrionmentMap(skyTexture);
	skybox->SetTexture(0, skyTexture);
	skybox->SetScale(glm::vec3(100.0f));
	skybox->SetPosition(glm::vec3(0.0, 0, 0));

	reflectionCube = new ReflectionCube(MeshType::Sphere, cam);
	skybox->SetName("reflectionCube");
	GLuint reflectProgram = ShaderManager::GetInstance()->GetProgram("Assets/Shaders/reflection.vs", "Assets/Shaders/reflection.fs");
	reflectionCube->SetProgram(reflectProgram);
	reflectionCube->SetTexture(0, skyTexture);
	reflectionCube->SetScale(glm::vec3(10.0f));
	reflectionCube->SetPosition(glm::vec3(-4.0, 0, 0));

	refractionCube = new ReflectionCube(MeshType::Sphere, cam);
	refractionCube->SetName("refractionCube");
	GLuint refractProgram = ShaderManager::GetInstance()->GetProgram("Assets/Shaders/reflection.vs", "Assets/Shaders/refraction.fs");
	refractionCube->SetProgram(refractProgram);
	refractionCube->SetTexture(0, skyTexture);
	refractionCube->SetScale(glm::vec3(10.0f));
	refractionCube->SetPosition(glm::vec3(-4.0, -2, 0));

	modelShder = ShaderManager::GetInstance()->GetProgram("Assets/Shaders/meshModel.vs", "Assets/Shaders/meshModel.fs");
	meshModel = new Model("BOSS_model_final.fbx");
	//meshModel = new Model("DarkElf01.3ds");
	meshModel->SetCamera(cam);
	meshModel->SetLight(light);
	meshModel->SetScale(glm::vec3(8.0f));
	meshModel->SetPosition(glm::vec3(0, -4, 0));

	meshModel2 = new Model("BOSS_model_final.fbx");
	meshModel2->SetCamera(cam);
	meshModel2->SetLight(light);
	meshModel2->SetScale(glm::vec3(8.0f));
	meshModel2->SetPosition(glm::vec3(-5, -4, 0));

	meshModel3 = new Model("BOSS_model_final.fbx");
	meshModel3->SetCamera(cam);
	meshModel3->SetLight(light);
	meshModel3->SetScale(glm::vec3(8.0f));
	meshModel3->SetPosition(glm::vec3(5, -4, 0));


	screenShader = ShaderManager::GetInstance()->GetProgram("Assets/Shaders/PostProcess.vs", "Assets/Shaders/PostProcess.fs");	
}

void Destroy()
{
	for (auto* renderer : renderList_)
	{
		delete renderer;
		renderer = nullptr;
	}
	renderList_.clear();

	for (auto* renderer : shadowRenderList_)
	{
		delete renderer;
		renderer = nullptr;
	}
	shadowRenderList_.clear();

	delete skybox;
	delete reflectionCube;
	delete refractionCube;

	delete meshModel;
	delete meshModel2;
	delete meshModel3;

	delete mesh;
	delete litMesh;
	delete cubeman;
	delete cubeman2;
	delete bottom;
	delete light;
	delete cam;
}

static void ErrorFunction(int id, const char* desc)
{
	std::cout << desc << std::endl;
}

//-------------------------------------------------------------------------------------
int main()
{
	glfwSetErrorCallback(&::ErrorFunction);

	glfwInit();
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

	GLFWwindow* window = glfwCreateWindow(1280, 720, "OpenGL4.5(SM5) Renderer", nullptr, nullptr);

	glfwMakeContextCurrent(window);

	glfwSetKeyCallback(window, ProcessKeyboard);
	glfwSetCursorPosCallback(window, ProcessMouseMove);
	glfwSetMouseButtonCallback(window, ProcessMouseButton);

	glewInit();

	InitScene();


	g_GLEngine->InitShadowmap();
	g_GLEngine->InitPostProcessing();


	unsigned int frameCnt = 0;
	double elapsedTime = 0;
	deltaTime = glfwGetTime();
	while (!glfwWindowShouldClose(window))
	{
		double beginTime = glfwGetTime();

		UpdateScene(deltaTime);

		isEnableWireFrame ? glPolygonMode(GL_FRONT_AND_BACK, GL_LINE) : glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);


		if (!useShadowmap)
		{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			if (!usePostProcessing)
			{
				PostRenderScene();
			}			
			else
			{
				PreRenderScene();				
				PostRenderScene();
			}
		}
		else
		{
			PreRenderScene();

			//render scene.	//쉐도우맵 그림자가 추가되면 2번 그려야 함. 부하가 2배
			PostRenderScene();
		}


		glfwSwapBuffers(window);
		glfwPollEvents();

		deltaTime = glfwGetTime() - beginTime;

		++frameCnt;
		elapsedTime += deltaTime;
		if (elapsedTime >= 1.0f) {

			float fps = (float)(frameCnt / elapsedTime);
			frameCnt = 0;
			elapsedTime = 0.0f;
			label->SetText(std::string("FPS : " + std::to_string((fps))));	//c++11 to_string
		}

		//label->SetText(std::string("FPS : " + std::to_string((deltaTime))));	//c++11 to_string		
	}

	Destroy();

	glfwTerminate();
	return 0;
}

void InitSceneForInstancing(GLuint texture)
{
	GLuint instancingShaderProgram = ShaderManager::GetInstance()->GetProgram("Assets/Shaders/InstancingShader.vs", "Assets/Shaders/InstancingShader.fs");
	//GLuint instancingShaderProgram = shaderLoader.CreateProgram("Assets/Shaders/InstancingShader.vs", "Assets/Shaders/InstancingShader.fs");	
	assert(instancingShaderProgram != GL_FALSE);
	instancingMesh = new LitInstanceMeshRenderer(MeshType::Sphere, cam, light);
	instancingMesh->SetProgram(instancingShaderProgram);
	instancingMesh->SetPosition(glm::vec3(0.f, 0.f, 0.f));
	instancingMesh->SetScale(glm::vec3(8.0f));
	instancingMesh->SetTexture(0, texture);


	//인스턴싱은 그릴 개체수만큼 생성하는 것이 아니라, 1개만 생성하고, 나머지는 개체수만큼 트랜스폼 정보가 있으면 된다.
	std::vector<glm::mat4> transformList;
	transformList.resize(amount);
	glm::vec3 basePos{ 0.0f, 0.0f, 0.0f };
	int posModFactor = COUNT_X * static_cast<int>((DistanceWithObject * 0.5f));
	for (int y = 0; y < COUNT_Y; ++y)
	{
		for (int x = 0; x < COUNT_X; ++x)
		{
			glm::mat4 model = glm::mat4(1.0f);

			//T
			//glm::vec3 tempPos = { basePos.x + (x * DistanceWithObject) - posModFactor, basePos.y + (y * DistanceWithObject) - posModFactor, basePos.z };
			model = glm::translate(model, { basePos.x + (x * DistanceWithObject) - posModFactor, basePos.y + (y * DistanceWithObject) - posModFactor, basePos.z });

			//S			
			model = glm::scale(model, glm::vec3(8.0f));

			//R			
			//model = glm::rotate(model, 10.0f, glm::vec3(1.0f, 1.0f, 1.0f));
			transformList[y * COUNT_Y + x] = model;
		}
	}

	instancingMesh->SetTransforms(std::move(transformList));
}

void RenderSceneForInstancing()
{
	instancingMesh->Draw();
}

void ProcessKeyboard(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}

	if (action == GLFW_PRESS)
	{
		printf("Press %d key\n", key);
	}
	else if (action == GLFW_RELEASE)
	{
		printf("Release %d key\n", key);

		if (key == GLFW_KEY_1)
		{
			isEnableWireFrame = !isEnableWireFrame;
		}
		else if (key == GLFW_KEY_2)
		{
			useInstancing = !useInstancing;
		}
		else if (key == GLFW_KEY_3)
		{
			useShadowmap = !useShadowmap;
		}
		else if (key == GLFW_KEY_4)
		{
			useOrthProjection = !useOrthProjection;
		}
		else if (key == GLFW_KEY_5)
		{
			useHierachySample = !useHierachySample;
			if (useHierachySample)
			{
				cam->SetPosition({ 0.0f, 10.0f, 10.0f });
			}
		}
		else if (key == GLFW_KEY_6)
		{
			usePostProcessing = !usePostProcessing;
		}		


	}
	else if (action == GLFW_REPEAT)
	{
		printf("Repeat %d key\n", key);
	}

	//update돌면서 키보드 콜백 등록안하는 경우 이렇게도 사용가능
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cam->ProcessKeyboard(GLFW_KEY_UP, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cam->ProcessKeyboard(GLFW_KEY_DOWN, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cam->ProcessKeyboard(GLFW_KEY_LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cam->ProcessKeyboard(GLFW_KEY_RIGHT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
	{
		glm::vec3 euler = cubeman->GetRotationEuler();
		euler += glm::vec3(0.0f, 10.0 * deltaTime, 0.0f);
		cubeman->SetRotation(euler);
	}
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
	{
		glm::vec3 euler = cubeman->GetRotationEuler();
		euler -= glm::vec3(0.0f, 10.0 * deltaTime, 0.0f);
		cubeman->SetRotation(euler);
	}
	if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
	{
		glm::vec3 euler = cubeman->GetRotationEuler();
		euler -= glm::vec3(10.0f * deltaTime, 0.0, 0.0f);
		cubeman->SetRotation(euler);
	}
	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
	{
		glm::vec3 euler = cubeman->GetRotationEuler();
		euler += glm::vec3(10.0f * deltaTime, 0.0, 0.0f);
		cubeman->SetRotation(euler);
	}
}

bool firstMouse = false;
double lastX;
double lastY;
void ProcessMouseMove(GLFWwindow* window, double xpos, double ypos)
{
	//printf("MouseMove %d %d\n", xpos, ypos);

	if (::firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		::firstMouse = false;
	}

	auto xoffset = xpos - lastX;
	auto yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	cam->ProcessMouseMovement(xoffset, yoffset);
}

void ProcessMouseButton(GLFWwindow* window, int button, int action, int mods)
{
	if (action == GLFW_PRESS)
	{
		printf("Press %d button\n", button);
	}
	else if (action == GLFW_RELEASE)
	{
		printf("Release %d button\n", button);
	}
}

