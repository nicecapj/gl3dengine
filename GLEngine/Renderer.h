#pragma once
#include <gl/glew.h>
#include "Dependencies/glm/glm.hpp"
#include "Dependencies/glm/gtc/matrix_transform.hpp"
#include "Dependencies/glm/gtc/type_ptr.hpp"

#include <vector>

#include "Mesh.h"

class Renderer
{
public:
    Renderer();
    virtual ~Renderer();

    virtual void Draw() = 0;
    virtual void UpdateScene(double deltaTimeMs);

    inline glm::vec3 GetPosition(){ return position_; }
    void SetPosition(glm::vec3 position);
	inline glm::vec3 GetScale() { return scale_; }
    void SetScale(glm::vec3 scale);

    void SetProgram(GLuint program);
    void SetTexture(GLuint textureID);
	
	inline glm::vec3 GetColor() { return color_; 	}
	void SetColor(glm::vec3 color);

	inline GLuint GetVAO() { return vao_; 	}	//used for instancing	
	inline GLsizei GetIndiciesSize() {
		return indicies_.size();
	}	
	inline GLuint GetProgram() { return program_; }
	inline GLuint GetTexture() {return texture_;	}
protected:
    glm::vec3 position_;
    glm::vec3 scale_;
    GLuint texture_;
    GLuint program_;
	glm::vec3 color_;

	std::vector<Vertex> vertices_;
	std::vector<GLuint> indicies_;

	GLuint vbo_;	//vertex buffer object. this is geometrical information, attributes such as position, normal, color, texture coordination. store on a per vertex base on GPU
	GLuint ebo_;	//element buffer object. this is used to store vertex index. such as index buffer of directX
	GLuint vao_;	//vertex array object. this is helper container object that store all VBOs and attributes. 오브젝트마다 1개이상의 VBO가 있을텐데, 각 프레임을 랜더링 할때마다, 바인드하긴 지루하니까 이걸 사용

	//buffer store in GPU. Modern GPU bandwidth is 600GB/s, Modern CPU bandwidth is 12GB/s
	//Buffer objects are used to store, retrive, move data.    
};

