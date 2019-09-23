#include "pch.h"
#include "SkyBoxRenderer.h"
#include "TextureManager.h"
#include "Camera.h"
#include <vector>


SkyBoxRenderer::SkyBoxRenderer(MeshType meshType, class Camera* camera)
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

	//VBO
	//���ؽ���� ������Ʈ�� �����, ���ø����̼ǿ� ���ε��ϸ�
	glGenVertexArrays(1, &vao_);
	glBindVertexArray(vao_);

	//���ؽ����� ������Ʈ�� ���� ���ε� �Ҽ� �ִ�. �޸� ���·� ����������, GL_ARRAY_BUFFER���� Ÿ�� ��ð� �ʿ��ϴ�.
	glGenBuffers(1, &vbo_);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_);

	//���ؽ����� ������Ʈ�� ��������� ���� �����͸� �Ѱܼ�. ���ۿ� �����Ų��.
	//GL_STATIC_DRAW�� �ѹ��� ����(����)�ϰ�, ��� ����ϴ� �����̴�.
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices_.size(), &vertices_[0], GL_STATIC_DRAW);

	//position�̳� normal���� �Ӽ��� ������, ��� ����ؾ� �ϴ� ���� �ƴϴ�.
	//�ʿ��� ���� position�� color�� �ʿ������� �̰͸� Ȱ��ȭ����
	glEnableVertexAttribArray(0);	//Vertex ����ü �Ӽ� 0�� ��� Ȱ��ȭ -> position���� ���

	//normalize�ȵ� ���̶� GL_FALSE
	glVertexAttribPointer(0,	//index of vertex array. ���ø����̼ǿ� ������ vao�� ���ε�Ǿ����� ��������, 0���� �� ������ ���
		3,	//x,y,z
		GL_FLOAT,	//type of xyz
		GL_FALSE,	//normalized. 0~1�� �ƴ� ���̸� false�Ѵ�
		sizeof(Vertex),	//stride�� �����̶� ������, ���ۿ��� ��� ������� �پ�鼭 �������� �����ϴ� ������
		(GLvoid*)0);	//���ؽ� struct�ȿ��� ���ؽ��Ӽ��� �ɼ�(position : 0, normal:1? 3? -> 12(pos:4x3))


//EBO
	glGenBuffers(1, &ebo_);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indicies_.size(), &indicies_[0], GL_STATIC_DRAW);

	//Unbind buffer and vertex array as a precaution
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}


SkyBoxRenderer::~SkyBoxRenderer()
{
}

void SkyBoxRenderer::GenerateCubemap(std::string cubemapName)
{
	//unsigned int textureID;
	//glGenTextures(1, &textureID);
	//glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	//std::vector<std::string> textures_faces;
	//textures_faces.push_back(cubemapName + "right");	//GL_TEXTURE_CUBE_MAP_POSITIVE_X
	//textures_faces.push_back(cubemapName + "left");	//GL_TEXTURE_CUBE_MAP_NEGATIVE_X
	//textures_faces.push_back(cubemapName + "top");	//GL_TEXTURE_CUBE_MAP_POSITIVE_Y
	//textures_faces.push_back(cubemapName + "bottom");	//GL_TEXTURE_CUBE_MAP_NEGATIVE_Y
	//textures_faces.push_back(cubemapName + "back");	//GL_TEXTURE_CUBE_MAP_POSITIVE_Z
	//textures_faces.push_back(cubemapName + "front");	//GL_TEXTURE_CUBE_MAP_NEGATIVE_Z	
	
	//int width, height, nrChannels;
	//unsigned char *data;
	//for (GLuint i = 0; i < textures_faces.size(); i++)
	//{
	//	TextureManager::GetTextureID()
	//	data = stbi_load(textures_faces[i].c_str(), &width, &height, &nrChannels, 0);
	//	glTexImage2D(
	//		GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
	//		0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
	//	);
	//}	
}

void SkyBoxRenderer::PreDraw()
{
	Renderer::PreDraw();
}

void SkyBoxRenderer::Draw()
{
	glUseProgram(program_);

	//model ->  view -> projection matrix�� uniform variables�� ���� ���̴��� ������ ������ �ִ�.
	//glGetUniformLocation()�Լ��� ���̴����α׷� �ȿ� ����� ������ �̸����� ������ �� �ִ�. ex) uniform mat4 model;
	GLuint modelLocation = glGetUniformLocation(program_, "model");
	//glUniformXXX������ �Լ��� ���̴���  ����� ������ ������, ���� ������ �� �ִ�.
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model_));	//����������, �ѱ浥������ ��, ��ġ���� ����, �ѱ� DATA�� ������

	glm::mat4 view = camera_->GetViewMatrix();
	glm::mat4 proj = camera_->GetProjectMatrix();
	glm::mat4 vp = proj * view;

	GLuint vpLocation = glGetUniformLocation(program_, "vp");	//uniform mat4 view;
	glUniformMatrix4fv(vpLocation, 1, GL_FALSE, glm::value_ptr(vp));

	glm::vec3 textureDir(1.0f, 0.0f, 0.0f);
	GLuint textureDirLocation = glGetUniformLocation(program_, "textureDir");	//uniform mat4 view;
	glUniformMatrix4fv(textureDirLocation, 1, GL_FALSE, glm::value_ptr(textureDir));
	

	//texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, GetTexture(0));


	//draw
	//�ѹ��� �׸� �����͸� ���� �䱸�Ѵ�.(vao_) ���� glDrawElements�� ���� �׸���.
	glBindVertexArray(vao_);
	glDrawElements(GL_TRIANGLES, (GLsizei)indicies_.size(), GL_UNSIGNED_INT, 0);

	//marks end of draw function
	glBindVertexArray(0);
	glUseProgram(0);

	Renderer::Draw();
}

void SkyBoxRenderer::PostDraw()
{
	Renderer::PostDraw();
}
