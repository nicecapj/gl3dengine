#include "pch.h"
#include "Renderer.h"
#include <algorithm>

int Renderer::objectCount = 0;

Renderer::Renderer()
{
	textures_.reserve(8);
	SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	SetScale(glm::vec3(1.0f, 1.0f, 1.0f));
	SetRotation(glm::vec3(0.0f, 0.0f, 0.0f));

	char buff[64];		
	snprintf(buff, 10, "%d", ++Renderer::objectCount);

	//itoa(Renderer::objectCount, buff, 10);	deprecated
	SetName("RenderObj_" + std::string(buff));
}


Renderer::~Renderer()
{	
	for (auto renderer : child_)
	{
		delete renderer;
	}
	child_.empty();
}

void Renderer::PreDraw()
{
	for (auto renderer : child_)
	{
		renderer->PreDraw();
	}
}

void Renderer::Draw()
{
	for (auto renderer : child_)
	{
		renderer->Draw();
	}
}

void Renderer::PostDraw()
{
	for (auto renderer : child_)
	{
		renderer->PostDraw();
	}
}

void Renderer::UpdateScene(glm::mat4* matParentModel, double deltaTimeMs)
{		
	glm::mat4 id(1.0);

	glm::mat4 transformMatrix = glm::translate(glm::mat4(1.0), position_);
	glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0), scale_);	
	//model_ = transformMatrix * matRot_ * scaleMatrix;	//이것은 dx방식.. gl은 열우선행렬임으로 SRT
	model_ = scaleMatrix * matRot_ * transformMatrix;	//이것은 dx방식.. gl은 열우선행렬임으로 SRT
	if (matParentModel)
	{
		model_ = (*matParentModel) * model_;		
	}	

	
	for (auto renderer : child_)
	{		
		renderer->UpdateScene(&model_, deltaTimeMs);
	}
}

void Renderer::SetPosition(glm::vec3 position)
{
    position_ = position;
}

void Renderer::SetScale(glm::vec3 scale)
{
    scale_ = scale;
}

void Renderer::SetRotation(glm::vec3 bias, float angle)
{
	glm::quat q(bias * angle);
	matRot_ = glm::mat4_cast(q);
}

void Renderer::SetRotation(glm::vec3 eulerAngles)
{	
	glm::quat q(eulerAngles);
	matRot_ = glm::mat4_cast(q);
}

glm::vec3 Renderer::GetRotationEuler()
{
	glm::quat q(matRot_);
	return glm::eulerAngles(q);
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

void Renderer::UpdateVertics(std::vector<Vertex>&& vertics)
{
	vertices_ = vertics;
	glBindBuffer(GL_ARRAY_BUFFER, vbo_);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices_.size(), &vertices_[0], GL_STATIC_DRAW);	//STATIC
	glBindBuffer(GL_ARRAY_BUFFER, 0);
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
