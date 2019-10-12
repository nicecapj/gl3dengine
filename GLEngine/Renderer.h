﻿#pragma once
#include <gl/glew.h>
#include "Dependencies/glm/glm.hpp"
#include "Dependencies/glm/gtc/matrix_transform.hpp"
#include "Dependencies/glm/gtc/type_ptr.hpp"

#include <vector>
#include <string.h>

#include "Mesh.h"

class Renderer
{
public:
    Renderer();
    virtual ~Renderer();

	virtual void PreDraw();
	virtual void Draw();
	virtual void PostDraw();
    virtual void UpdateScene(glm::mat4* matParentModel, double deltaTimeMs);

    inline glm::vec3 GetPosition(){ return position_; }
    void SetPosition(glm::vec3 position);
	inline glm::vec3 GetScale() { return scale_; }
    void SetScale(glm::vec3 scale);
	void SetRotation(glm::vec3 bias, float angle);
	void SetRotation(glm::vec3 eulerAngles);
	const glm::mat4& GetRotation() { return matRot_; }
	glm::vec3 GetRotationEuler();

	inline GLuint GetProgram() { return program_; }
	void SetProgram(GLuint program);

	inline GLuint GetTexture(int index) { return textures_[index]; }
	virtual void SetTexture(int index, GLuint textureID);

	inline glm::vec3 GetColor() { return color_; 	}
	void SetColor(glm::vec3 color);

	inline GLuint GetVAO() { return vao_; 	}	//used for instancing	
	inline GLsizei GetIndiciesSize() {
		return (GLsizei)indices_.size();
	}			
	void UpdateVertics(std::vector<Vertex>&& vertics);

	std::vector<Vertex>& GetVertics() { return vertices_; }
	std::vector<GLuint>& GetIndicis() { return indices_; }

	virtual void SetEnableDynamicShadow(bool isEnable);

	void AddChild(Renderer* renderer);
	void RemoveChild(Renderer* renderer);

	//이게 있으면 디버깅할때 대상 산정하기 좋다.
	void SetName(std::string name) {
		name_ = name;
	};	
	std::string GetName() { return name_; }
protected:
	glm::vec3 position_ = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 scale_ = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::mat4 matRot_;
	glm::mat4 model_;

    std::vector<GLuint> textures_;
    GLuint program_;
	glm::vec3 color_;

	std::vector<Vertex> vertices_;
	std::vector<GLuint> indices_;

	GLuint vbo_;	//vertex buffer object. this is geometrical information, attributes such as position, normal, color, texture coordination. store on a per vertex base on GPU
	GLuint ebo_;	//element buffer object. this is used to store vertex index. such as index buffer of directX
	GLuint vao_;	//vertex array object. this is helper container object that store all VBOs and attributes. 오브젝트마다 1개이상의 VBO가 있을텐데, 각 프레임을 랜더링 할때마다, 바인드하긴 지루하니까 이걸 사용

	//buffer store in GPU. Modern GPU bandwidth is 600GB/s, Modern CPU bandwidth is 12GB/s
	//Buffer objects are used to store, retrive, move data.    

	Renderer* parent_;
	std::vector< Renderer*> child_;

	std::string name_;

	bool enableDynamicShadow_ = false;

	static int objectCount;
};

