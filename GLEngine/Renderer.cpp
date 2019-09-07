#include "pch.h"
#include "Renderer.h"


Renderer::Renderer()
{
	textures_.reserve(8);
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
