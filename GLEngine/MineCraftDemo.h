#pragma once
#include "Application.h"
class MineCraftDemo :
	public Application
{
public:
	MineCraftDemo();
	virtual ~MineCraftDemo();

	virtual void Initialze() override;
	virtual void Finalize() override;
	virtual void UpdateScene(double deltaTimeMs) override;
	virtual void RenderScene() override;

	virtual void ProcessKeyboard(GLFWwindow* window, int key, int scancode, int action, int mods) override;
protected:
	virtual void PostRenderScene() override;
	virtual void PreRenderScene() override;

private:
	class CubemanRenderer* cubeman = nullptr;
	class CubemanRenderer* cubeman2 = nullptr;
	class SkyBoxRenderer* skybox = nullptr;	

	class Camera* cam_ = nullptr;
	class LightRenderer* light_ = nullptr;
	class MeshRenderer* debugQuad = nullptr;

	double deltaTimeMs_ = 0.0;
};

