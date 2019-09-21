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
	//root_->AddChild(body);	//지금처럼 스케일 적용하면, 부모 스케일 적용해서 커진다.
	AddChild(body);

	auto leftArm = new LitMeshRenderer(MeshType::Cube, camera, light);
	leftArm->SetProgram(program);
	leftArm->SetTexture(0, widowTex);
	leftArm->SetPosition(glm::vec3(-15.0f, -25.0f, 0.0f));
	leftArm->SetScale(glm::vec3(5.0f, 15.0f, 5.0f));
	auto leftArmVertics = leftArm->GetVertics();
	Mesh::setMineCraftLeftArmUV(leftArmVertics);
	leftArm->UpdateVertics(std::move(leftArmVertics));
	//root_->AddChild(leftArm);
	AddChild(leftArm);

	auto rightArm = new LitMeshRenderer(MeshType::Cube, camera, light);
	rightArm->SetProgram(program);
	rightArm->SetTexture(0, widowTex);
	rightArm->SetPosition(glm::vec3(15.0f, -25.0f, 0.0f));
	rightArm->SetScale(glm::vec3(5.0f, 15.0f, 5.0f));
	auto rightArmVertics = rightArm->GetVertics();
	Mesh::setMineCraftRightArmUV(rightArmVertics);
	rightArm->UpdateVertics(std::move(rightArmVertics));
	//root_->AddChild(rightArm);
	AddChild(rightArm);

	auto leftLeg = new LitMeshRenderer(MeshType::Cube, camera, light);
	leftLeg->SetProgram(program);
	leftLeg->SetTexture(0, widowTex);
	leftLeg->SetPosition(glm::vec3(-5.0f, -55.0f, 0.0f));
	leftLeg->SetScale(glm::vec3(5.0f, 15.0f, 5.0f));
	auto leftLegVertics = leftLeg->GetVertics();
	Mesh::setMineCraftLegUV(leftLegVertics);
	leftLeg->UpdateVertics(std::move(leftLegVertics));
	//root_->AddChild(leftLeg);
	AddChild(leftLeg);

	auto rightLeg = new LitMeshRenderer(MeshType::Cube, camera, light);
	rightLeg->SetProgram(program);
	rightLeg->SetTexture(0, widowTex);
	rightLeg->SetPosition(glm::vec3(5.0f, -55.0f, 0.0f));
	rightLeg->SetScale(glm::vec3(5.0f, 15.0f, 5.0f));
	auto rightLegVertics = rightLeg->GetVertics();
	Mesh::setMineCraftLegUV(rightLegVertics);
	rightLeg->UpdateVertics(std::move(rightLegVertics));
//	root_->AddChild(rightLeg);
	AddChild(rightLeg);
}

CubemanRenderer::~CubemanRenderer()
{
}

void CubemanRenderer::PreDraw()
{
	Renderer::PreDraw();
}

void CubemanRenderer::Draw()
{
	Renderer::Draw();
}

void CubemanRenderer::PostDraw()
{
	Renderer::PostDraw();
}

void CubemanRenderer::SetEnableDynamicShadow(bool isEnable)
{
	for (auto renderer : child_)
	{
		renderer->SetEnableDynamicShadow(isEnable);
	}
}
