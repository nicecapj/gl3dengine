// GLEngine.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include "pch.h"
#include <iostream>
#include <GL/eglew.h>	//opengl extention wrangler
#include <GLFW/glfw3.h>	//OpenGL, window and input
#include <gl/glew.h>

#include "Mesh.h"
#include "LightRenderer.h"
#include "MeshRenderer.h"
#include "LitMeshRenderer.h"
#include "LitInstanceMeshRenderer.h"
#include "LitMeshShadowRenderer.h"
#include "Camera.h"
#include "ShaderLoader.h"
#include "TextureLoader.h"
#include "TextRenderer.h"

#include "Renderer.h"
#include <map>
#include <vector>
#include <assert.h>

const int COUNT_X = 10;
const int COUNT_Y = 10;
const float DistanceWithObject = 16.0f;
size_t amount = COUNT_Y * COUNT_X;

const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
const unsigned int SCR_WIDTH = 1280, SCR_HEIGHT = 720;

double deltaTime = 0;
bool isEnableWireFrame = false;
bool useInstancing = false;
bool useShadowmap = true;
bool useOrthProjection = false;

Camera* cam = nullptr;
LightRenderer* light = nullptr;
MeshRenderer* mesh = nullptr;
LitMeshRenderer* litMesh = nullptr;
LitMeshRenderer* bottom = nullptr;
TextRenderer* label = nullptr;
LitMeshShadowRenderer* depthMesh = nullptr;
MeshRenderer* debugQuad = nullptr;


LitInstanceMeshRenderer* instancingMesh = nullptr;
GLuint depthTextureShader;

GLuint depthMapFBO;
GLuint depthMap;

GLuint instancingBuffer = -1;

std::vector<Renderer*> renderList_;	
//std::map<Renderer*, int> instancingRendererMap_;


GLuint sphereTexture = -1;

void InitSceneForInstancing(ShaderLoader& shaderLoader, GLuint texture);
void ProcessKeyboard(GLFWwindow* window, int key, int scancode, int action, int mods);
void ProcessMouseMove(GLFWwindow* window, double xpos, double ypos);
void ProcessMouseButton(GLFWwindow* window, int button, int action, int mods);


void InitShadowmap();
void InitPhysics() {}

void RenderScene()
{	
    light->Draw();
    mesh->Draw();
    litMesh->Draw();
	bottom->Draw();
//    label->Draw();
	
	if (useShadowmap)
	{
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			return;

//		depthMesh->Draw();
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
	}	
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

	if (useInstancing)
	{
		instancingMesh->UpdateScene(deltaTimeMs);
	}	
}

void InitScene()
{
    glEnable(GL_DEPTH_TEST);
	cam = new Camera(45.0f, 1280.f, 720.f, 0.1f, 1000.0f, { 0.0f, 6.0f, 100.0f });

    //쉐이더는 컴파일하면 여러 지오메트리에서 공유할 수 있다.
    //매번 컴파일할것없이 부모만 컴파일해서 사용하면 된다. 여기서 언리얼의 메터리얼인스턴스 개념도 출발한다.
    //Draw할때, 파라미터(uniform)에 해당하는 것만 바꾸면 된다.
    ShaderLoader shaderLoader;
    GLuint shaderProgram = shaderLoader.CreateProgram("Assets/Shaders/FlatModel.vs", "Assets/Shaders/FlatModel.fs");
	assert(shaderProgram != GL_FALSE);

    light = new LightRenderer(MeshType::Sphere, cam);
    light->SetProgram(shaderProgram);	
    light->SetPosition({ 19.f, 19.f, 13.0f });


    //unlit static mesh
    GLuint textureShaderProgram = shaderLoader.CreateProgram("Assets/Shaders/TexturedModel.vs", "Assets/Shaders/TexturedModel.fs");
	assert(textureShaderProgram != GL_FALSE);
	TextureLoader textureLoader;
    //텍스처 역시 캐시해서 공유가능하다.
    //GLuint sphereTexture = textureLoader.GetTextureID("Assets/Textures/globe.dds");
	sphereTexture = textureLoader.GetTextureID("Assets/Textures/globe.dds");	
	
    mesh = new MeshRenderer(MeshType::Sphere, cam);
    mesh->SetProgram(textureShaderProgram);
    mesh->SetPosition({ 0.0f, 0.0f, 0.0f });
    mesh->SetScale(glm::vec3(8.0f));
	mesh->SetTexture(sphereTexture);
	
	
    //dynamic text
    GLuint textProgram = shaderLoader.CreateProgram("Assets/Shaders/text.vs", "Assets/Shaders/text.fs");
	assert(textProgram != GL_FALSE);
	label = new TextRenderer("Text", "Assets/fonts/DMSerifDisplay-Regular.ttf", 64, { 0.0f, 0.0f, 1.0f }, textProgram);
    label->SetProgram(textProgram);
    label->SetPosition(glm::vec2(320.0f, 500.0f));
    

    //lit static mesh
    GLuint textureLightShaderProgram = shaderLoader.CreateProgram("Assets/Shaders/litTexturedModel.vs", "Assets/Shaders/litTexturedModel.fs");
	assert(textureLightShaderProgram != GL_FALSE);
	litMesh = new LitMeshRenderer(MeshType::Sphere, cam, light);    
	litMesh->SetProgram(textureLightShaderProgram);
	
    litMesh->SetPosition({ 16.0f, 0.0f, 0.0f });
    litMesh->SetScale(glm::vec3(8.0f));
    litMesh->SetTexture(sphereTexture);

		
	bottom = new LitMeshRenderer(MeshType::Cube, cam, light);
	bottom->SetProgram(textureLightShaderProgram);
	bottom->SetPosition({ 0.0f, -20.0f, 0.0f });
	bottom->SetScale(glm::vec3(100.0f, 2.0f, 100.0f));
	GLuint groundTexture = textureLoader.GetTextureID("Assets/Textures/ground.dds");
	bottom->SetTexture(groundTexture);


	//shadowmap
	depthTextureShader = shaderLoader.CreateProgram("Assets/Shaders/depthTextureShader.vs", "Assets/Shaders/depthTextureShader.fs");	
	depthMesh = new LitMeshShadowRenderer(MeshType::Sphere, cam, light);
	depthMesh->SetProgram(textureShaderProgram);
	depthMesh->SetPosition({ 32.0f, 0.0f, 0.0f });
	depthMesh->SetScale(glm::vec3(8.0f));
	depthMesh->SetTexture(depthTextureShader);

	GLuint depthTextureDebugShaderProgram = shaderLoader.CreateProgram("Assets/Shaders/depthTextureDebug.vs", "Assets/Shaders/depthTextureDebug.fs");
	debugQuad = new MeshRenderer(MeshType::Cube, cam);
	debugQuad->SetProgram(shaderProgram);
	debugQuad->SetPosition({ -40.0f, 10.0f, -20.0f });
	debugQuad->SetScale(glm::vec3(10.0f));
	debugQuad->SetTexture(sphereTexture);
	debugQuad->isDebug = false;


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
			renderer->SetTexture(sphereTexture);

			renderList_.push_back(std::move(renderer));
		}
	}

	InitSceneForInstancing(shaderLoader, sphereTexture);		

}

void Destroy()
{
	for (auto* renderer : renderList_)
	{
		delete renderer;
		renderer = nullptr;
	}
	renderList_.clear();

	delete mesh;
	delete litMesh;
	delete light;
	delete bottom;
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

	GLFWwindow* window = glfwCreateWindow(1280, 720, "OpenGL4.5(SM5) Renderer", nullptr, nullptr);

    glfwMakeContextCurrent(window);

	glfwSetKeyCallback(window, ProcessKeyboard);
	glfwSetCursorPosCallback(window, ProcessMouseMove);
	glfwSetMouseButtonCallback(window, ProcessMouseButton);

    glewInit();
    
	InitScene();


	//InitShadowmap();

// The framebuffer, which regroups 0, 1, or more textures, and 0 or 1 depth buffer.
	GLuint FramebufferName = 0;
	glGenFramebuffers(1, &FramebufferName);
	glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);

	// Depth texture. Slower than a depth buffer, but you can sample it later in your shader
	GLuint depthTexture;
	glGenTextures(1, &depthTexture);
	glBindTexture(GL_TEXTURE_2D, depthTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, 1024, 1024, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthTexture, 0);

	glDrawBuffer(GL_NONE); // No color buffer is drawn to.

	// Always check that our framebuffer is ok
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		return false;


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
			RenderScene();
		}
		else
		{
			// 1. first render to depth map
			glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
			glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glClearColor(0.0, 0.0, 0.0, 1.0);

			//render scene.	//쉐도우맵 그림자가 추가되면 2번 그려야 함. 부하가 2배
			RenderScene();
			glBindFramebuffer(GL_FRAMEBUFFER, 0);

			// 2. then render scene as normal with shadow mapping (using depth map)
			glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			//ConfigureShaderAndMatrices();		
			//glBindTexture(GL_TEXTURE_2D, depthMap);
			debugQuad->SetTexture(depthMap);

			RenderScene();
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

void InitSceneForInstancing(ShaderLoader& shaderLoader, GLuint texture)
{	
	GLuint instancingShaderProgram = shaderLoader.CreateProgram("Assets/Shaders/InstancingShader.vs", "Assets/Shaders/InstancingShader.fs");	
	assert(instancingShaderProgram != GL_FALSE);
	instancingMesh = new LitInstanceMeshRenderer(MeshType::Sphere, cam, light);
	instancingMesh->SetProgram(instancingShaderProgram);
	instancingMesh->SetPosition(glm::vec3(0.f, 0.f, 0.f));
	instancingMesh->SetScale(glm::vec3(8.0f));
	instancingMesh->SetTexture(texture);	


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
}

bool firstMouse = false;
double lastX;
double lastY;
void ProcessMouseMove(GLFWwindow* window, double xpos, double ypos)
{
	printf("MouseMove %d %d\n", xpos, ypos);

	if (::firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		::firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

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


void InitShadowmap()
{	
	glGenFramebuffers(1, &depthMapFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);

	//깊이 텍스쳐는 깊이버퍼보다 느리지만, 쉐이더에서 텍스처 샘플로 사용할수 있는 장점이 있다.	
	//unsigned int depthMap;
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	
	//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthMap, 0);

	glDrawBuffer(GL_NONE);	//버퍼에 컬러 그리지 않음
	glReadBuffer(GL_NONE);	//버퍼에서 컬러 읽지 않음.


	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	//return;
}

