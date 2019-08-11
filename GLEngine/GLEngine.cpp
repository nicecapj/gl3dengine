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

Camera* cam = nullptr;
LightRenderer* light = nullptr;
MeshRenderer* mesh = nullptr;
LitMeshRenderer* litMesh = nullptr;
LitMeshRenderer* bottom = nullptr;
TextRenderer* label = nullptr;
LitInstanceMeshRenderer* instancingMesh = nullptr;

std::vector<Renderer*> renderList_;	
//std::map<Renderer*, int> instancingRendererMap_;
std::vector<glm::mat4> transformList_;

void InitSceneForInstancing(ShaderLoader& shaderLoader, GLuint texture);
void RenderSceneForInstancing();

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
		//renderer->Draw();
	}

	RenderSceneForInstancing();
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
			model = glm::scale(model, glm::vec3((float)((rand() % 20) + 1)));

			//R			
			model = glm::rotate(model, 10.0f, glm::vec3(1.0f, 1.0f, 1.0f));
			transformList_[y * COUNT_Y + x] = model;
		}
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

    //dynamic text
    GLuint textProgram = shaderLoader.CreateProgram("Assets/Shaders/text.vs", "Assets/Shaders/text.fs");
	assert(textProgram != GL_FALSE);
	label = new TextRenderer("Text", "Assets/fonts/DMSerifDisplay-Regular.ttf", 64, { 0.0f, 0.0f, 1.0f }, textProgram);
    label->SetProgram(textProgram);
    label->SetPosition(glm::vec2(320.0f, 500.0f));

    mesh->SetTexture(sphereTexture);

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
	bool isEnableWireFrame = false;
    glfwSetErrorCallback(&::ErrorFunction);

    glfwInit();

	GLFWwindow* window = glfwCreateWindow(1280, 720, "OpenGL4.5(SM5) Renderer", nullptr, nullptr);

    glfwMakeContextCurrent(window);

    glewInit();
    
	InitScene();

	unsigned int frameCnt = 0; 
	double elapsedTime = 0; 
	double deltaTime = glfwGetTime();
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

void InitSceneForInstancing(ShaderLoader& shaderLoader, GLuint texture)
{
	size_t amount = COUNT_Y * COUNT_X;

	GLuint instancingShaderProgram = shaderLoader.CreateProgram("Assets/Shaders/InstancingShader.vs", "Assets/Shaders/InstancingShader.fs");	
	assert(instancingShaderProgram != GL_FALSE);
	instancingMesh = new LitInstanceMeshRenderer(MeshType::Sphere, cam);
	instancingMesh->SetProgram(instancingShaderProgram);
	instancingMesh->SetPosition(glm::vec3(0.f, 0.f, 0.f));
	instancingMesh->SetScale(glm::vec3(8.0f));
	instancingMesh->SetTexture(texture);


	//인스턴싱은 그릴 개체수만큼 생성하는 것이 아니라, 1개만 생성하고, 나머지는 개체수만큼 트랜스폼 정보가 있으면 된다.
	//instancingRendererMap_.insert(std::make_pair(renderer, amount));
	transformList_.resize(amount);

	//instancing transform buffer
	//https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glEnableVertexAttribArray.xhtml
	GLuint buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, amount * sizeof(glm::mat4), &transformList_[0], GL_STATIC_DRAW);
	GLsizei vec4Size = sizeof(glm::vec4);
	unsigned int vao = instancingMesh->GetVAO();
	
	//uniform으로 접근 안하고, 정점 속성(Vertex Attribute)으로 접근하고 싶은데 최대 지원이 vec4임으로 4번에 vs로 넘겨야 한다.
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, vec4Size, (GLvoid*)0);
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, vec4Size, (GLvoid*)vec4Size);
	glEnableVertexAttribArray(5);
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, vec4Size, (GLvoid*)(2 * vec4Size));
	glEnableVertexAttribArray(6);
	glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, vec4Size, (GLvoid*)(3 * vec4Size));

	//modify the rate at which generic vertex attributes advance during instanced rendering		
	glVertexAttribDivisor(3, 1);
	glVertexAttribDivisor(4, 1);
	glVertexAttribDivisor(5, 1);
	glVertexAttribDivisor(6, 1);

	glBindVertexArray(0);	
}

void RenderSceneForInstancing()
{
	glm::mat4 view = cam->GetViewMatrix();
	glm::mat4 proj = cam->GetProjectMatrix();
	glm::mat4 vp = proj * view;

	glm::mat4 transformMatrix = glm::translate(glm::mat4(1.0), instancingMesh->GetPosition());
	glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0), instancingMesh->GetScale());
	glm::mat4 model = transformMatrix * scaleMatrix;

	//set shader
	GLuint program = instancingMesh->GetProgram();
	glUseProgram(program);

	GLuint modelLocation = glGetUniformLocation(program, "model");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));	//유니폼변수, 넘길데이터의 수, 전치인지 여부, 넘길 DATA의 포인터

	GLuint vpLocation = glGetUniformLocation(program, "vp");	//uniform mat4 view;
	glUniformMatrix4fv(vpLocation, 1, GL_FALSE, glm::value_ptr(vp));

	glBindTexture(GL_TEXTURE_2D, instancingMesh->GetTexture());

	glBindVertexArray(instancingMesh->GetVAO());
	glDrawElementsInstanced(GL_TRIANGLES, instancingMesh->GetIndiciesSize(), GL_UNSIGNED_INT, 0, renderList_.size());
	glBindVertexArray(0);
}