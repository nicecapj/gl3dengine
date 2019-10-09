#include "pch.h"
#include "DynamicFontDemo.h"
#include "TextRenderer.h"
#include <assert.h>


DynamicFontDemo::DynamicFontDemo()
{
}


DynamicFontDemo::~DynamicFontDemo()
{
}

void DynamicFontDemo::Initialze()
{
	//dynamic text
	GLuint textProgram = ShaderManager::GetInstance()->GetProgram("Assets/Shaders/text.vs", "Assets/Shaders/text.fs");
	assert(textProgram != GL_FALSE);
	label_ = new TextRenderer("Text", "Assets/fonts/DMSerifDisplay-Regular.ttf", 64, { 0.0f, 0.0f, 1.0f }, textProgram);
	label_->SetProgram(textProgram);
	label_->SetPosition(glm::vec2(320.0f, 500.0f));
}

void DynamicFontDemo::Finalize()
{
	delete label_;
}

void DynamicFontDemo::UpdateScene(double deltaTimeMs)
{
	
}

void DynamicFontDemo::RenderScene()
{
	Application::RenderScene();
}

void DynamicFontDemo::ProcessKeyboard(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	
}

void DynamicFontDemo::PostRenderScene()
{
	label_->Draw();
}

void DynamicFontDemo::PreRenderScene()
{
	
}
