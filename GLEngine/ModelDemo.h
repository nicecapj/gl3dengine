#pragma once
#include "Application.h"

class ModelDemo :
	public Application
{
public:
	ModelDemo();
	virtual ~ModelDemo();

	virtual void Initialze() override;
	virtual void Finalize() override;
	virtual void UpdateScene(double deltaTimeMs) override;
	virtual void RenderScene() override;
	virtual void ProcessKeyboard(GLFWwindow* window, int key, int scancode, int action, int mods) override;
	virtual class Camera* GetCamera() override;

protected:
	virtual void PostRenderScene() override;


	virtual void PreRenderScene() override;

	class Model* meshModel_ = nullptr;
	GLuint modelShder_ = -1;
	GLuint screenShader_ = -1;
	
	class Camera* cam_ = nullptr;
	class LightRenderer* light_ = nullptr;
	class MeshRenderer* debugQuad = nullptr;

	class SkyBoxRenderer* skybox = nullptr;
	class ReflectionCube* reflectionCube = nullptr;
	class ReflectionCube* refractionCube = nullptr;
};

