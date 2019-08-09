#include "pch.h"
#include "Renderer.h"


Renderer::Renderer()
{
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

void Renderer::SetTexture(GLuint textureID)
{
    texture_ = textureID;
}

void Renderer::SetColor(glm::vec3 color)
{
	color_ = color;
}