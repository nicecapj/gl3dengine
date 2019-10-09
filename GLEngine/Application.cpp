#include "pch.h"
#include "Application.h"
#include "Camera.h"


Application::Application()
{
}


Application::~Application()
{
	Finalize();
}

Camera* Application::GetCamera()
{
	return nullptr;
}

void Application::RenderScene()
{
	PreRenderScene();
	PostRenderScene();
}
