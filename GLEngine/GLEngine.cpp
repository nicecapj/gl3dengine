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
#include "Camera.h"
#include "ShaderLoader.h"
#include "TextureLoader.h"
#include "TextRenderer.h"

#include "Renderer.h"

const int COUNT_X = 10;
const int COUNT_Y = 10;
const float DistanceWithObject = 16.0f;

Camera* cam = nullptr;
LightRenderer* light = nullptr;
MeshRenderer* mesh = nullptr;
LitMeshRenderer* litMesh = nullptr;
LitMeshRenderer* bottom = nullptr;
TextRenderer* label = nullptr;

std::vector<Renderer*> renderList_;	//부모로 묶어서 해야하지만, 프로토는 이정도면 된다.

void InitPhysics()
{
}

void RenderScene()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0, 0.0, 0.0, 1.0);

    light->Draw();
    mesh->Draw();
    litMesh->Draw();
	bottom->Draw();
    label->Draw();

	//인스턴싱 안 했을떄 코드
	for (auto* renderer : renderList_)
	{
		renderer->Draw();
	}
    //draw
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

    light = new LightRenderer(MeshType::Sphere, cam);
    light->SetProgram(shaderProgram);	
    light->SetPosition({ 0.f, 0.f, -20.0f });


    //unlit static mesh
    GLuint textureShaderProgram = shaderLoader.CreateProgram("Assets/Shaders/TexturedModel.vs", "Assets/Shaders/TexturedModel.fs");
    TextureLoader textureLoader;
    //텍스처 역시 캐시해서 공유가능하다.
    GLuint sphereTexture = textureLoader.GetTextureID("Assets/Textures/globe.dds");
    mesh = new MeshRenderer(MeshType::Sphere, cam);
    mesh->SetProgram(textureShaderProgram);
    mesh->SetPosition({ 0.0f, 0.0f, 0.0f });
    mesh->SetScale(glm::vec3(8.0f));

    //dynamic text
    GLuint textProgram = shaderLoader.CreateProgram("Assets/Shaders/text.vs", "Assets/Shaders/text.fs");
    label = new TextRenderer("Text", "Assets/fonts/DMSerifDisplay-Regular.ttf", 64, { 0.0f, 0.0f, 1.0f }, textProgram);
    label->SetProgram(textProgram);
    label->SetPosition(glm::vec2(320.0f, 500.0f));

    mesh->SetTexture(sphereTexture);

    //lit static mesh
    GLuint textureLightShaderProgram = shaderLoader.CreateProgram("Assets/Shaders/litTexturedModel.vs", "Assets/Shaders/litTexturedModel.fs");
    litMesh = new LitMeshRenderer(MeshType::Sphere, cam, light);
    litMesh->SetProgram(textureLightShaderProgram);
    litMesh->SetPosition({ 16.0f, 0.0f, 0.0f });
    litMesh->SetScale(glm::vec3(8.0f));
    litMesh->SetTexture(sphereTexture);

		
	bottom = new LitMeshRenderer(MeshType::Cube, cam, light);
	bottom->SetProgram(textureLightShaderProgram);
	bottom->SetPosition({ 0.0f, -20.0f, 0.0f });
	bottom->SetScale(glm::vec3(20.0f, 2.0f, 20.0f));
	bottom->SetTexture(sphereTexture);

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

    glewInit();

    InitScene();

	double deltaTime = glfwGetTime();
    while (!glfwWindowShouldClose(window))
    {
		double beginTime = glfwGetTime();
		
		UpdateScene(deltaTime);

        RenderScene();

        glfwSwapBuffers(window);
        glfwPollEvents();

		deltaTime = glfwGetTime() - beginTime;

		label->SetText(std::string("FPS : " + std::to_string((deltaTime))));	//c++11 to_string
    }

	Destroy();

    glfwTerminate();
    return 0;
}

