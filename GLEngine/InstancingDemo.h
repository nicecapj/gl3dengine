#pragma once
#include "Application.h"
#include<vector>

class InstancingDemo :
	public Application
{
public:
	const int COUNT_X = 10;
	const int COUNT_Y = 10;
	const float DistanceWithObject = 16.0f;	
	size_t amount = COUNT_Y * COUNT_X;

	InstancingDemo();
	virtual ~InstancingDemo();

	virtual void Initialze() override;
	virtual void Finalize() override;
	virtual void UpdateScene(double deltaTimeMs) override;
	virtual void RenderScene() override;
	virtual void ProcessKeyboard(GLFWwindow* window, int key, int scancode, int action, int mods) override;


	virtual class Camera* GetCamera() override;

protected:
	virtual void PostRenderScene() override;
	virtual void PreRenderScene() override;
	
	class LightRenderer* light_ = nullptr;
	class Camera* cam_ = nullptr;

	bool useInstancing = false;
	class LitInstanceMeshRenderer* instancingMesh_ = nullptr;
	
	std::vector<class Renderer*> renderList_;		
};

