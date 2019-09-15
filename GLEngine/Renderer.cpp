#include "pch.h"
#include "Renderer.h"
#include <algorithm>

int Renderer::objectCount = 0;

Renderer::Renderer()
{
	textures_.reserve(8);
	SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	SetScale(glm::vec3(1.0f, 1.0f, 1.0f));

	char buff[64];		
	snprintf(buff, 10, "%d", ++Renderer::objectCount);

	//itoa(Renderer::objectCount, buff, 10);	deprecated
	SetName("RenderObj_" + std::string(buff));
}


Renderer::~Renderer()
{	
}

void Renderer::UpdateScene(double deltaTimeMs)
{

}

void Renderer::SetPosition(glm::vec3 position)
{
    position_ = position;
}

void Renderer::SetScale(glm::vec3 scale)
{
    scale_ = scale;
}

void Renderer::SetProgram(GLuint program)
{
    program_ = program;
}

void Renderer::SetTexture(int index, GLuint textureID)
{
	if (textures_.size() <= index)
	{
		textures_.push_back(textureID);
	}
	textures_[index] = textureID;
}

void Renderer::SetColor(glm::vec3 color)
{
	color_ = color;
}

void Renderer::SetEnableDynamicShadow(bool isEnable)
{
	enableDynamicShadow_ = isEnable;
}

void Renderer::AddChild(Renderer* renderer)
{
	renderer->parent_ = this;
	child_.push_back(renderer);
}

void Renderer::RemoveChild(Renderer* renderer)
{
	auto it = std::find(child_.begin(), child_.end(), renderer);
	if (it != child_.end())
	{
		child_.erase(it);
	}		
}
