#include "pch.h"
#include "LitInstanceMeshRenderer.h"
#include "Camera.h"
#include "LightRenderer.h"
#include <GLFW/glfw3.h>

const int COUNT_X = 10;
const int COUNT_Y = 10;
const float DistanceWithObject = 16.0f;
LitInstanceMeshRenderer::LitInstanceMeshRenderer(MeshType meshType, Camera* camera,  LightRenderer* light)
    :Renderer()
{
    camera_ = camera;	
	light_ = light;

	switch (meshType)
	{
	case MeshType::Trangile:
	{
		Mesh::setTriData(vertices_, indicies_);
	} break;
	case MeshType::Cube:
	{
		Mesh::setCubeData(vertices_, indicies_);
	} break;
	case MeshType::Sphere:
	{
		Mesh::setSphereData(vertices_, indicies_);
	} break;
	case MeshType::Quad:
	{
		Mesh::setQuadData(vertices_, indicies_);
	} break;
	}

	//VAO
	glGenVertexArrays(1, &vao_);
	glBindVertexArray(vao_);

	//VBO
	glGenBuffers(1, &vbo_);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices_.size(), &vertices_[0], GL_STATIC_DRAW);	//STATIC


	//Vertex안의 맴버의 순서는 중요하지 않다. 지정한 인덱스가 쉐이더 안에서 layout (location = 0)로 정확하게
	//가르키는 것이 중요하다. 중간에 맴버를 건너뛰거나 생략해도 상관없다.
	glEnableVertexAttribArray(0);	//VS : layout (location = 0) in vec3 position;
	glVertexAttribPointer(0,
		3,
		GL_FLOAT,
		GL_FALSE,
		sizeof(Vertex),
		(GLvoid*)0);

	glEnableVertexAttribArray(1);	//layout (location = 1) in vec2 texCoord;
	glVertexAttribPointer(1,
		2,
		GL_FLOAT,
		GL_FALSE,
		sizeof(Vertex),
		(GLvoid*)(offsetof(Vertex, Vertex::texCoords)));

	glEnableVertexAttribArray(2);	//VS : layout (location = 2) in vec3 normal;
	glVertexAttribPointer(2,
		3,
		GL_FLOAT,
		GL_FALSE,
		sizeof(Vertex),
		(GLvoid*)(offsetof(Vertex, Vertex::normal)));

	//EBO
	glGenBuffers(1, &ebo_);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indicies_.size(), &indicies_[0], GL_STATIC_DRAW);

	//Unbind buffer and vertex array as a precaution
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}


LitInstanceMeshRenderer::~LitInstanceMeshRenderer()
{

}

void LitInstanceMeshRenderer::Draw()
{
	glm::mat4 view = camera_->GetViewMatrix();
	glm::mat4 proj = camera_->GetProjectMatrix();
	glm::mat4 vp = proj * view;

	//set shader	
	glUseProgram(program_);

	GLuint vpLocation = glGetUniformLocation(program_, "vp");	//uniform mat4 view;
	glUniformMatrix4fv(vpLocation, 1, GL_FALSE, glm::value_ptr(vp));

	auto camPos = camera_->GetCameraPosition();
	GLuint camPosLocation = glGetUniformLocation(program_, "cameraPos");	//uniform vec3 cameraPos;
	assert(camPosLocation != -1);
	glUniform3f(camPosLocation, camPos.x, camPos.y, camPos.z);

	GLuint lightPosLocation = glGetUniformLocation(program_, "lightPos");	//uniform vec3 lightPos;
	//assert(lightPosLocation != -1);
	auto lightPos = light_->GetPosition();
	glUniform3f(lightPosLocation, lightPos.x, lightPos.y, lightPos.z);


	glBindTexture(GL_TEXTURE_2D, texture_);

	glBindVertexArray(vao_);
	glDrawElementsInstanced(GL_TRIANGLES, GetIndiciesSize(), GL_UNSIGNED_INT, 0, objectCount_);
	glBindVertexArray(0);
}

void LitInstanceMeshRenderer::UpdateScene(double deltaTimeMs)
{

}

void LitInstanceMeshRenderer::SetObjectCount(GLuint count)
{
	objectCount_ = count;
}
