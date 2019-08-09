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

Camera* cam = nullptr;
LightRenderer* light = nullptr;
MeshRenderer* mesh = nullptr;
LitMeshRenderer* litMesh = nullptr;
LitMeshRenderer* bottom = nullptr;
TextRenderer* label = nullptr;

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
    //draw
}

void InitScene()
{
    glEnable(GL_DEPTH_TEST);
    cam = new Camera(45.0f, 1280.f, 1024.f, 0.1f, 100.0f, { 0.0f, 6.0f, -40.0f });

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

    GLFWwindow* window = glfwCreateWindow(1280, 1024, "OpenGL4.5(SM5) Renderer", nullptr, nullptr);

    glfwMakeContextCurrent(window);

    glewInit();

    InitScene();

    while (!glfwWindowShouldClose(window))
    {
        RenderScene();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

	delete bottom;
	delete litMesh;
	delete label;
	delete light;
    delete cam;

    glfwTerminate();
    return 0;
}

