#include "pch.h"
#include "InstancingDemo.h"
#include "Renderer.h"
#include "LightRenderer.h"
#include "LitMeshRenderer.h"
#include "LitInstanceMeshRenderer.h"

InstancingDemo::InstancingDemo()
{
}


InstancingDemo::~InstancingDemo()
{
}

void InstancingDemo::Initialze()
{
	GLuint textureLightShaderProgram = ShaderManager::GetInstance()->GetProgram("Assets/Shaders/litTexturedModel.vs", "Assets/Shaders/litTexturedModel.fs");
	assert(textureLightShaderProgram != GL_FALSE);

	GLuint sphereTexture = TextureManager::GetInstance()->GetTextureID("Assets/Textures/globe.dds");

	//normal case
	glm::vec3 basePos{ 0.0f, 0.0f, 0.0f };
	int posModFactor = COUNT_X * static_cast<int>((DistanceWithObject * 0.5f));
	for (int y = 0; y < COUNT_Y; ++y)
	{
		for (int x = 0; x < COUNT_X; ++x)
		{
			auto renderer = new LitMeshRenderer(MeshType::Sphere, cam_, light_);

			renderer->SetProgram(textureLightShaderProgram);	//�����ϵ� ���̴� ����

			//��ġ��, ȸ��, �����ϸ��� �����Ӱ� �ٲܼ� �ִ�.
			renderer->SetPosition(glm::vec3(basePos.x + (x * DistanceWithObject) - posModFactor, basePos.y + (y * DistanceWithObject) - posModFactor, basePos.z));
			renderer->SetScale(glm::vec3(8.0f));

			//�ؽ��ĵ� �̸� �ε��س��� �����Ҽ� �ִ�. �ٸ� draw�Ҷ� gpu�� ������ �ȴ�.
			renderer->SetTexture(0, sphereTexture);

			renderList_.push_back(std::move(renderer));
		}
	}


	//instancing case
	GLuint instancingShaderProgram = ShaderManager::GetInstance()->GetProgram("Assets/Shaders/InstancingShader.vs", "Assets/Shaders/InstancingShader.fs");
	//GLuint instancingShaderProgram = shaderLoader.CreateProgram("Assets/Shaders/InstancingShader.vs", "Assets/Shaders/InstancingShader.fs");	
	assert(instancingShaderProgram != GL_FALSE);
	instancingMesh_ = new LitInstanceMeshRenderer(MeshType::Sphere, cam_, light_);
	instancingMesh_->SetProgram(instancingShaderProgram);
	instancingMesh_->SetPosition(glm::vec3(0.f, 0.f, 0.f));
	instancingMesh_->SetScale(glm::vec3(8.0f));
	instancingMesh_->SetTexture(0, sphereTexture);


	//�ν��Ͻ��� �׸� ��ü����ŭ �����ϴ� ���� �ƴ϶�, 1���� �����ϰ�, �������� ��ü����ŭ Ʈ������ ������ ������ �ȴ�.
	std::vector<glm::mat4> transformList;
	transformList.resize(amount);	
	//glm::vec3 basePos{ 0.0f, 0.0f, 0.0f };
	//int posModFactor = COUNT_X * static_cast<int>((DistanceWithObject * 0.5f));
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

	instancingMesh_->SetTransforms(std::move(transformList));
}

void InstancingDemo::Finalize()
{
	for (auto* renderer : renderList_)
	{
		delete renderer;
		renderer = nullptr;
	}
	renderList_.clear();

	delete cam_;
	delete light_;
}

void InstancingDemo::UpdateScene(double deltaTimeMs)
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


	if (useInstancing)
	{
		instancingMesh_->UpdateScene(nullptr, deltaTimeMs);
	}
}

void InstancingDemo::RenderScene()
{
	Application::RenderScene();
}

void InstancingDemo::ProcessKeyboard(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_2)
	{
		useInstancing = !useInstancing;
	}
}

void InstancingDemo::PostRenderScene()
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
		instancingMesh_->Draw();
	}
}

void InstancingDemo::PreRenderScene()
{
	throw std::logic_error("The method or operation is not implemented.");
}
