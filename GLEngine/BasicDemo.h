#pragma once
#include "Application.h"
class BasicDemo :
	public Application
{
public:
	BasicDemo();
	virtual ~BasicDemo();

	virtual void Initialze() override;
	virtual void Finalize() override;
	virtual void UpdateScene(double deltaTimeMs) override;
	virtual void RenderScene() override;
	virtual void ProcessKeyboard(GLFWwindow* window, int key, int scancode, int action, int mods) override;

protected:
	virtual void PostRenderScene() override;
	virtual void PreRenderScene() override;


private:	
	class MeshRenderer* mesh_ = nullptr;
	class LightRenderer* light_ = nullptr;
	class LitMeshRenderer* litMesh_ = nullptr;
	class LitMeshRenderer* bottom_ = nullptr;
	class Camera* cam_ = nullptr;
};

