// GLEngine.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include "pch.h"
#include <iostream>
#include <GL/eglew.h>	//opengl extention wrangler
#include <GLFW/glfw3.h>	//OpenGL, window and input
#include <gl/glew.h>

#include "GLEngine.h"
#include "Camera.h"

#include <string>
#include <assert.h>

#include "TextRenderer.h"		//test ok
#include "BasicDemo.h"			//test ok
#include "InstancingDemo.h"		//test ok
#include "ShadowmapDemo.h"
#include "MineCraftDemo.h"		//ok
#include "ModelDemo.h"			//ok
#include "DynamicFontDemo.h"	//ok - apply to FPS
#include "PostProcessginDemo.h"

double deltaTime = 0;
bool isEnableWireFrame = false;
bool useOrthProjection = false;
TextRenderer* label = nullptr;

InstancingDemo gameInstance;
void ProcessKeyboard(GLFWwindow* window, int key, int scancode, int action, int mods);
void ProcessMouseMove(GLFWwindow* window, double xpos, double ypos);
void ProcessMouseButton(GLFWwindow* window, int button, int action, int mods);


void UpdateScene(double deltaTimeMs)
{
	auto cam = gameInstance.GetCamera();
	if (cam)
	{
		if (useOrthProjection)
		{
			cam->SetOrthProjection(-100, 100, -100, 100, -10, 1000);
		}
		else
		{
			cam->SetPerspectiveProjection(45.0f, 1280.f, 720.f, 0.1f, 1000.0f);
		}
	}


	gameInstance.UpdateScene(deltaTimeMs);
}

void Destroy()
{
	delete label;
}

static void ErrorFunction(int id, const char* desc)
{
	std::cout << desc << std::endl;
}


void InitFpsLabel()
{
	//dynamic text
	GLuint textProgram = ShaderManager::GetInstance()->GetProgram("Assets/Shaders/text.vs", "Assets/Shaders/text.fs");
	assert(textProgram != GL_FALSE);
	label = new TextRenderer("Text", "Assets/fonts/DMSerifDisplay-Regular.ttf", 64, { 0.0f, 0.0f, 1.0f }, textProgram);
	label->SetProgram(textProgram);
	label->SetPosition(glm::vec2(320.0f, 500.0f));
}

void RenderScene()
{	
	gameInstance.RenderScene();
	
	label->Draw();

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
	
	g_GLEngine->InitShadowmap();
	g_GLEngine->InitPostProcessing();

	InitFpsLabel();
	gameInstance.Initialze();


	unsigned int frameCnt = 0;
	double elapsedTime = 0;
	deltaTime = glfwGetTime();
	while (!glfwWindowShouldClose(window))
	{
		double beginTime = glfwGetTime();

		UpdateScene(deltaTime);		
		isEnableWireFrame ? glPolygonMode(GL_FRONT_AND_BACK, GL_LINE) : glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		RenderScene();
		
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


void ProcessKeyboard(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}

	gameInstance.ProcessKeyboard(window, key, scancode, action, mods);

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
	
	auto cam = gameInstance.GetCamera();
	if (cam)
	{
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			cam->ProcessKeyboard(GLFW_KEY_UP, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			cam->ProcessKeyboard(GLFW_KEY_DOWN, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			cam->ProcessKeyboard(GLFW_KEY_LEFT, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			cam->ProcessKeyboard(GLFW_KEY_RIGHT, deltaTime);
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

	auto cam = gameInstance.GetCamera();
	if (cam)
	{
		cam->ProcessMouseMovement(xoffset, yoffset);
	}	
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

