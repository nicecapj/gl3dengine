#include "pch.h"
#include "CubemanRenderer.h"
#include "Camera.h"
//#include "LitMeshShadowRenderer.h"
#include "LitMeshRenderer.h"
#include "LightRenderer.h"

CubemanRenderer::CubemanRenderer(GLuint shader, class Camera* camera, class LightRenderer* light)
{
	light_ = light;
	camera_ = camera;
	SetProgram(shader);

	root_ = new LitMeshRenderer(MeshType::Cube, camera, light);	//head
	root_->SetProgram(shader);
	root_->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	root_->SetScale(glm::vec3(4));
	AddChild(root_);

	auto body = new LitMeshRenderer(MeshType::Cube, camera, light);
	body->SetProgram(shader);
	body->SetPosition(glm::vec3(0.0f, -8.0f, 0.0f));
	body->SetScale(glm::vec3(4.0f));
	AddChild(body);

	auto leftArm = new LitMeshRenderer(MeshType::Cube, camera, light);
	leftArm->SetProgram(shader);
	leftArm->SetPosition(glm::vec3(-8.0f, -10.0f, 0.0f));
	leftArm->SetScale(glm::vec3(2.0f, 8.0f, 2.0f));
	AddChild(leftArm);

	auto rightArm = new LitMeshRenderer(MeshType::Cube, camera, light);
	rightArm->SetPosition(glm::vec3(8.0f, -10.0f, 0.0f));
	rightArm->SetScale(glm::vec3(2.0f, 8.0f, 2.0f));
	rightArm->SetProgram(shader);
	AddChild(rightArm);

	auto leftLeg = new LitMeshRenderer(MeshType::Cube, camera, light);
	leftLeg->SetProgram(shader);
	leftLeg->SetPosition(glm::vec3(-4.0f, -24.0f, 0.0f));
	leftLeg->SetScale(glm::vec3(2.0f, 8.0f, 2.0f));
	AddChild(leftLeg);

	auto rightLeg = new LitMeshRenderer(MeshType::Cube, camera, light);
	rightLeg->SetProgram(shader);
	rightLeg->SetPosition(glm::vec3(4.0f, -24.0f, 0.0f));
	rightLeg->SetScale(glm::vec3(2.0f, 8.0f, 2.0f));
	AddChild(rightLeg);
}

CubemanRenderer::~CubemanRenderer()
{
	for (auto renderer : child_)
	{
		delete renderer;
	}
	child_.empty();
}

void CubemanRenderer::PreDraw()
{
	for (auto renderer : child_)
	{
		renderer->PreDraw();
	}
}

void CubemanRenderer::Draw()
{
	for (auto renderer : child_)
	{
		renderer->Draw();
	}
}

void CubemanRenderer::PostDraw()
{
	for (auto renderer : child_)
	{
		renderer->PostDraw();
	}
}

void CubemanRenderer::UpdateScene(double deltaTimeMs)
{
	for (auto renderer : child_)
	{
		renderer->UpdateScene(deltaTimeMs);
	}
}

void CubemanRenderer::SetProgram(GLuint program)
{
	for (auto renderer : child_)
	{
		renderer->SetProgram(program);
	}
}

void CubemanRenderer::SetTexture(int index, GLuint textureID)
{
	for (auto renderer : child_)
	{
		renderer->SetTexture(index, textureID);
	}
}

void CubemanRenderer::SetEnableDynamicShadow(bool isEnable)
{
	for (auto renderer : child_)
	{
		renderer->SetEnableDynamicShadow(isEnable);
	}
}