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
	GLuint widowTex = TextureManager::GetInstance()->GetTextureID("Assets/Textures/bdafn-lyofa.dds");


	auto head = new LitMeshRenderer(MeshType::Cube, camera, light);	//head
	head->SetProgram(program);
	head->SetTexture(0, widowTex);
	auto vertics = head->GetVertics();
	Mesh::setMineCraftFaceUV(vertics);
	head->UpdateVertics(std::move(vertics));

	head->SetPosition(glm::vec3(0.0f, 2.5f, 0.0f));
	head->SetScale(glm::vec3(1.0f, 1.0f, 1.0f));
	head->SetName("Head");
	//body->AddChild(head);
	AddChild(head);

	auto body = new LitMeshRenderer(MeshType::Cube, camera, light);
	body->SetProgram(program);
	body->SetTexture(0, widowTex);
	body->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	body->SetScale(glm::vec3(1.0f, 1.5f, 0.5f));	
	auto bodyVertics = body->GetVertics();
	Mesh::setMineCraftBodyUV(bodyVertics);
	body->UpdateVertics(std::move(bodyVertics));	
	body->SetName("body");
	AddChild(body);		

	auto leftArm = new LitMeshRenderer(MeshType::Cube, camera, light);
	leftArm->SetProgram(program);
	leftArm->SetTexture(0, widowTex);
	leftArm->SetPosition(glm::vec3(-3.0f, 0.0f, 0.0f));
	leftArm->SetScale(glm::vec3(0.5f, 1.5f, 0.5f));
	auto leftArmVertics = leftArm->GetVertics();
	Mesh::setMineCraftLeftArmUV(leftArmVertics);
	leftArm->UpdateVertics(std::move(leftArmVertics));
	leftArm->SetName("leftArm");
	AddChild(leftArm);	
	//body->AddChild(leftArm);

	auto rightArm = new LitMeshRenderer(MeshType::Cube, camera, light);
	rightArm->SetProgram(program);
	rightArm->SetTexture(0, widowTex);
	rightArm->SetPosition(glm::vec3(3.0f, 0.0f, 0.0f));
	rightArm->SetScale(glm::vec3(0.5f, 1.5f, 0.5f));
	auto rightArmVertics = rightArm->GetVertics();
	Mesh::setMineCraftRightArmUV(rightArmVertics);
	rightArm->UpdateVertics(std::move(rightArmVertics));
	rightArm->SetName("rightArm");
	//body->AddChild(rightArm);
	AddChild(rightArm);

	auto leftLeg = new LitMeshRenderer(MeshType::Cube, camera, light);
	leftLeg->SetProgram(program);
	leftLeg->SetTexture(0, widowTex);
	leftLeg->SetPosition(glm::vec3(-1.0f, -2.0f, 0.0f));
	leftLeg->SetScale(glm::vec3(0.5f, 1.5f, 0.5f));
	auto leftLegVertics = leftLeg->GetVertics();
	Mesh::setMineCraftLegUV(leftLegVertics);
	leftLeg->UpdateVertics(std::move(leftLegVertics));
	leftLeg->SetName("leftLeg");
	//body->AddChild(leftLeg);
	AddChild(leftLeg);

	auto rightLeg = new LitMeshRenderer(MeshType::Cube, camera, light);
	rightLeg->SetProgram(program);
	rightLeg->SetTexture(0, widowTex);
	rightLeg->SetPosition(glm::vec3(1.0f, -2.0f, 0.0f));
	rightLeg->SetScale(glm::vec3(0.5f, 1.5f, 0.5f));
	auto rightLegVertics = rightLeg->GetVertics();
	Mesh::setMineCraftLegUV(rightLegVertics);
	rightLeg->UpdateVertics(std::move(rightLegVertics));
	rightLeg->SetName("rightLeg");
	//body->AddChild(rightLeg);
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
