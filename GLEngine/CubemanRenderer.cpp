#include "pch.h"
#include "CubemanRenderer.h"
#include "Camera.h"
//#include "LitMeshShadowRenderer.h"
#include "LitMeshRenderer.h"
#include "LightRenderer.h"
#include "ShaderManager.h"
#include "TextureManager.h"

CubemanRenderer::CubemanRenderer(class Camera* camera, class LightRenderer* light)
{	
	light_ = light;
	camera_ = camera;

	GLuint program = ShaderManager::GetInstance()->GetProgram("Assets/Shaders/litTexturedModel.vs", "Assets/Shaders/litTexturedModel.fs");
	SetProgram(program);

	root_ = new LitMeshRenderer(MeshType::Cube, camera, light);	//head
	root_->SetProgram(program);
	GLuint widowTex = TextureManager::GetInstance()->GetTextureID("Assets/Textures/bdafn-lyofa.dds");
	root_->SetTexture(0, widowTex);
	auto vertics = root_->GetVertics();
	Mesh::setMineCraftFaceUV(vertics);
	root_->UpdateVertics(std::move(vertics));

	root_->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	root_->SetScale(glm::vec3(10.0f));

	AddChild(root_);
	
	auto body = new LitMeshRenderer(MeshType::Cube, camera, light);
	body->SetProgram(program);
	body->SetTexture(0, widowTex);
	body->SetScale(glm::vec3(10.0f, 15.0f, 10.0f));
	body->SetPosition(glm::vec3(0.0f, -25.0f, 0.0f));	
	auto bodyVertics = body->GetVertics();
	Mesh::setMineCraftBodyUV(bodyVertics);
	body->UpdateVertics(std::move(bodyVertics));
	AddChild(body);

	auto leftArm = new LitMeshRenderer(MeshType::Cube, camera, light);
	leftArm->SetProgram(program);
	leftArm->SetTexture(0, widowTex);
	leftArm->SetPosition(glm::vec3(-15.0f, -25.0f, 0.0f));
	leftArm->SetScale(glm::vec3(5.0f, 15.0f, 5.0f));
	auto leftArmVertics = leftArm->GetVertics();
	Mesh::setMineCraftLeftArmUV(leftArmVertics);
	leftArm->UpdateVertics(std::move(leftArmVertics));
	AddChild(leftArm);

	auto rightArm = new LitMeshRenderer(MeshType::Cube, camera, light);
	rightArm->SetProgram(program);
	rightArm->SetTexture(0, widowTex);
	rightArm->SetPosition(glm::vec3(15.0f, -25.0f, 0.0f));
	rightArm->SetScale(glm::vec3(5.0f, 15.0f, 5.0f));
	auto rightArmVertics = rightArm->GetVertics();
	Mesh::setMineCraftRightArmUV(rightArmVertics);
	rightArm->UpdateVertics(std::move(rightArmVertics));
	AddChild(rightArm);

	auto leftLeg = new LitMeshRenderer(MeshType::Cube, camera, light);
	leftLeg->SetProgram(program);
	leftLeg->SetTexture(0, widowTex);
	leftLeg->SetPosition(glm::vec3(-5.0f, -55.0f, 0.0f));
	leftLeg->SetScale(glm::vec3(5.0f, 15.0f, 5.0f));
	auto leftLegVertics = leftLeg->GetVertics();
	Mesh::setMineCraftLegUV(leftLegVertics);
	leftLeg->UpdateVertics(std::move(leftLegVertics));
	AddChild(leftLeg);

	auto rightLeg = new LitMeshRenderer(MeshType::Cube, camera, light);
	rightLeg->SetProgram(program);
	rightLeg->SetTexture(0, widowTex);
	rightLeg->SetPosition(glm::vec3(5.0f, -55.0f, 0.0f));
	rightLeg->SetScale(glm::vec3(5.0f, 15.0f, 5.0f));
	auto rightLegVertics = rightLeg->GetVertics();
	Mesh::setMineCraftLegUV(rightLegVertics);
	rightLeg->UpdateVertics(std::move(rightLegVertics));
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
		//renderer->SetRotation(GetRotationEuler());
		renderer->UpdateScene(deltaTimeMs);
	}
}

//void CubemanRenderer::SetProgram(GLuint program)
//{
//	for (auto renderer : child_)
//	{
//		renderer->SetProgram(program);
//	}
//}

void CubemanRenderer::SetEnableDynamicShadow(bool isEnable)
{
	for (auto renderer : child_)
	{
		renderer->SetEnableDynamicShadow(isEnable);
	}
}
