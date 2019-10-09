#pragma once
#include <GL/eglew.h>	//opengl extention wrangler
#include <GLFW/glfw3.h>	//OpenGL, window and input
#include <gl/glew.h>

class Application
{
public:
	Application();
	virtual ~Application();

	virtual class Camera* GetCamera();

	virtual void Initialze() {}
	virtual void Finalize() {}

	virtual void UpdateScene(double deltaTimeMs) = 0;
	virtual void RenderScene();

	virtual void ProcessKeyboard(GLFWwindow* window, int key, int scancode, int action, int mods) {}
protected:
	virtual void PostRenderScene() {}
	virtual void PreRenderScene() {}
};