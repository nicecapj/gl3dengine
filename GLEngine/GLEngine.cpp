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
bool useShadowmap = false;

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
std::vector<glm::mat4> transformList_;

void InitSceneForInstancing(ShaderLoader& shaderLoader, GLuint texture);
void RenderSceneForInstancing();
void ProcessKeyboard(GLFWwindow* window, int key, int scancode, int action, int mods);
void ProcessMouseMove(GLFWwindow* window, double xpos, double ypos);
void ProcessMouseButton(GLFWwindow* window, int button, int action, int mods);

void InitPhysics()
{
}

void RenderScene()
{	
    light->Draw();
    mesh->Draw();
    litMesh->Draw();
	bottom->Draw();
    label->Draw();
	
	if (useShadowmap)
	{
		depthMesh->Draw();
		debugQuad->Draw();
	}	
	
	if (!useInstancing)
	{
		for (auto* renderer : renderList_)
		{
			renderer->Draw();
		}
	}
	else
	{
		RenderSceneForInstancing();
	}    
}


void UpdateScene(double deltaTimeMs)
{	
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

	//glBindVertexArray(instancingMesh->GetVAO());	
	//glBindBuffer(GL_ARRAY_BUFFER, instancingBuffer);

	////인스턴싱은 실시간 갱신 안하고, 스태틱으로 선언하였음.
	////움직이는 처리는 버텍스쉐이더에서(윈드등에 반응하는 잔디등)
	//glm::vec3 basePos{ 0.0f, 0.0f, 0.0f };
	//int posModFactor = COUNT_X * static_cast<int>((DistanceWithObject * 0.5f));
	//for (int y = 0; y < COUNT_Y; ++y)
	//{
	//	for (int x = 0; x < COUNT_X; ++x)
	//	{
	//		int index = y * COUNT_Y + x;
	//		glm::mat4 model = glm::mat4(1.0f);
	//			
	//		double posFactorX = glm::sin(glfwGetTime() + (index * deltaTimeMs));
	//		double posFactorY = glm::cos(glfwGetTime() + (index * deltaTimeMs));

	//		auto pos = instancingMesh->GetPosition();
	//		//pos = { pos.x, pos.y, pos.z + posFactorX };

	//		//T
	//		//glm::vec3 tempPos = { basePos.x + (x * DistanceWithObject) - posModFactor, basePos.y + (y * DistanceWithObject) - posModFactor, basePos.z };
	//		model = glm::translate(model, { pos.x, pos.y, pos.z + posFactorX - cam->GetCameraPosition().z });

	//		//S			
	//		//model = glm::scale(model, glm::vec3((float)((rand() % 20) + 1)));
	//		model = glm::scale(model, glm::vec3(8.0f));

	//		//R			
	//		model = glm::rotate(model, 10.0f, glm::vec3(1.0f, 1.0f, 1.0f));
	//		transformList_[index] = model;
	//	}
	//}

	//glBufferData(GL_ARRAY_BUFFER, COUNT_X * COUNT_Y * sizeof(glm::mat4), &transformList_[0], GL_STATIC_DRAW);
	//glBindVertexArray(0);
}

void InitScene()
{
    glEnable(GL_DEPTH_TEST);
	cam = new Camera(45.0f, 1280.f, 720.f, 0.1f, 1000.0f, { 0.0f, 6.0f, -100.0f });

    //쉐이더는 컴파일하면 여러 지오메트리에서 공유할 수 있다.
    //매번 컴파일할것없이 부모만 컴파일해서 사용하면 된다. 여기서 언리얼의 메터리얼인스턴스 개념도 출발한다.
    //Draw할때, 파라미터(uniform)에 해당하는 것만 바꾸면 된다.
    ShaderLoader shaderLoader;
    GLuint shaderProgram = shaderLoader.CreateProgram("Assets/Shaders/FlatModel.vs", "Assets/Shaders/FlatModel.fs");
	assert(shaderProgram != GL_FALSE);

    light = new LightRenderer(MeshType::Sphere, cam);
    light->SetProgram(shaderProgram);	
    light->SetPosition({ 0.f, 0.f, -20.0f });


    //unlit static mesh
    GLuint textureShaderProgram = shaderLoader.CreateProgram("Assets/Shaders/TexturedModel.vs", "Assets/Shaders/TexturedModel.fs");
	assert(textureShaderProgram != GL_FALSE);
	TextureLoader textureLoader;
    //텍스처 역시 캐시해서 공유가능하다.
    GLuint sphereTexture = textureLoader.GetTextureID("Assets/Textures/globe.dds");
    mesh = new MeshRenderer(MeshType::Sphere, cam);
    mesh->SetProgram(textureShaderProgram);
    mesh->SetPosition({ 0.0f, 0.0f, 0.0f });
    mesh->SetScale(glm::vec3(8.0f));
	mesh->SetTexture(sphereTexture);
	
	
	GLuint depthTextureDebugShaderProgram = shaderLoader.CreateProgram("Assets/Shaders/depthTextureDebug.vs", "Assets/Shaders/depthTextureDebug.fs");
	debugQuad = new MeshRenderer(MeshType::Cube, cam);
	debugQuad->SetProgram(depthTextureDebugShaderProgram);
	debugQuad->SetPosition({ 0.0f, 0.0f, 0.0f });
	debugQuad->SetScale(glm::vec3(8.0f));
	//debugQuad->SetTexture(sphereTexture);

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
	bottom->SetScale(glm::vec3(50.0f, 2.0f, 50.0f));
	bottom->SetTexture(sphereTexture);

	depthTextureShader = shaderLoader.CreateProgram("Assets/Shaders/depthTextureShader.vs", "Assets/Shaders/depthTextureShader.fs");	
	depthMesh = new LitMeshShadowRenderer(MeshType::Sphere, cam, light);
	depthMesh->SetProgram(textureShaderProgram);
	depthMesh->SetPosition({ 32.0f, 0.0f, 0.0f });
	depthMesh->SetScale(glm::vec3(8.0f));
	depthMesh->SetTexture(depthTextureShader);

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
	instancingMesh->SetObjectCount(amount);


	//인스턴싱은 그릴 개체수만큼 생성하는 것이 아니라, 1개만 생성하고, 나머지는 개체수만큼 트랜스폼 정보가 있으면 된다.
	//instancingRendererMap_.insert(std::make_pair(renderer, amount));
	transformList_.resize(amount);
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
			transformList_[y * COUNT_Y + x] = model;
		}
	}

	unsigned int vao = instancingMesh->GetVAO();
	glBindVertexArray(vao);


	//instancing transform buffer
	//https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glEnableVertexAttribArray.xhtml
//	GLuint instancingBuffer;
	glGenBuffers(1, &instancingBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, instancingBuffer);
	glBufferData(GL_ARRAY_BUFFER, amount * sizeof(glm::mat4), &transformList_[0], GL_STATIC_DRAW);
	GLsizei vec4Size = sizeof(glm::vec4);

	//uniform으로 접근 안하고, 정점 속성(Vertex Attribute)으로 접근하고 싶은데 최대 지원이 vec4임으로 4번에 vs로 넘겨야 한다.
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)0);
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(vec4Size));
	glEnableVertexAttribArray(5);
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(2 * vec4Size));
	glEnableVertexAttribArray(6);
	glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(3 * vec4Size));

	//modify the rate at which generic vertex attributes advance during instanced rendering		
	glVertexAttribDivisor(3, 1);
	glVertexAttribDivisor(4, 1);
	glVertexAttribDivisor(5, 1);
	glVertexAttribDivisor(6, 1);

	glBindVertexArray(0);	
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
	unsigned int depthMapFBO;
	glGenFramebuffers(1, &depthMapFBO);

	//깊이 텍스쳐는 깊이버퍼보다 느리지만, 쉐이더에서 텍스처 샘플로 사용할수 있는 장점이 있다.	
	unsigned int depthMap;
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
		SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);	//color사용안함 설정.
	glReadBuffer(GL_NONE);	//color사용안함 설정.
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

