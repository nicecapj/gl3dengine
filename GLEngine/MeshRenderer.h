#pragma once

#include <gl/glew.h>

#include "Mesh.h"
#include "ShaderLoader.h"
#include "Camera.h"

#include "Dependencies/glm/glm.hpp"
#include "Dependencies/glm/gtc/matrix_transform.hpp"
#include "Dependencies/glm/gtc/type_ptr.hpp"

#include <vector>

class MeshRenderer
{
public:
	MeshRenderer(MeshType meshType, Camera* camera);
	~MeshRenderer();

	void Draw();

	void SetPosition(glm::vec3 position);
	void SetScale(glm::vec3 scale);
	
	void SetProgram(GLuint program);
	void SetTexture(GLuint textureID);	

private:
	Camera* camera_;

	std::vector<Vertex> vertices_;
	std::vector<GLuint> indicies_;

	glm::vec3 position_;
	glm::vec3 scale_;
	
	GLuint vao_;
	GLuint vbo_;
	GLuint ebo_;
	GLuint texture_;
	GLuint program_;

};

