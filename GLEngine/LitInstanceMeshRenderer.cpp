#include "pch.h"
#include "LitInstanceMeshRenderer.h"
#include <GLFW/glfw3.h>

const int COUNT_X = 10;
const int COUNT_Y = 10;
const float DistanceWithObject = 16.0f;
LitInstanceMeshRenderer::LitInstanceMeshRenderer(MeshType meshType, Camera* camera)
    :Renderer()
{
    camera_ = camera;	

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
	glm::mat4 transformMatrix = glm::translate(glm::mat4(1.0), position_);
	glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0), scale_);
	glm::mat4 model = transformMatrix * scaleMatrix;


	//set shader
	glUseProgram(program_);

	//model ->  view -> projection matrix를 uniform variables를 통해 쉐이더로 정보를 보낼수 있다.
	//glGetUniformLocation()함수로 쉐이더프로그램 안에 선언된 변수의 이름으로 가져올 수 있다. ex) uniform mat4 model;
	GLuint modelLocation = glGetUniformLocation(program_, "model");
	//glUniformXXX형식의 함수로 쉐이더에  연결된 유니폼 변수에, 값을 설정할 수 있다.
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));	//유니폼변수, 넘길데이터의 수, 전치인지 여부, 넘길 DATA의 포인터

	glm::mat4 view = camera_->GetViewMatrix();
	glm::mat4 proj = camera_->GetProjectMatrix();
	glm::mat4 vp = proj * view;

	GLuint vpLocation = glGetUniformLocation(program_, "vp");	//uniform mat4 view;
	glUniformMatrix4fv(vpLocation, 1, GL_FALSE, glm::value_ptr(vp));


    //texture
    glBindTexture(GL_TEXTURE_2D, texture_);

    //draw
    //한번만 그릴 데이터를 전부 요구한다.(vao_) 이후 glDrawElements를 통해 그린다.
    glBindVertexArray(vao_);

    glDrawElements(GL_TRIANGLES, indicies_.size(), GL_UNSIGNED_INT, 0);
	//glDrawElementsInstanced(GL_TRIANGLES, indicies_.size(), GL_UNSIGNED_INT, 0, 100);

    //marks end of draw function
    glBindVertexArray(0);
    glUseProgram(0);
}

void LitInstanceMeshRenderer::UpdateScene(double deltaTimeMs)
{

}