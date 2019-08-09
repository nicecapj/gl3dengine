#pragma once
#include <gl/glew.h>
#include "Dependencies/glm/glm.hpp"
#include "Dependencies/glm/gtc/matrix_transform.hpp"
#include "Dependencies/glm/gtc/type_ptr.hpp"

class Renderer
{
public:
    Renderer();
    virtual ~Renderer();

    virtual void Draw() = 0;
    virtual void UpdateScene(double deltaTimeMs);

    inline glm::vec3 GetPosition(){ return position_; }
    void SetPosition(glm::vec3 position);
    void SetScale(glm::vec3 scale);

    void SetProgram(GLuint program);
    void SetTexture(GLuint textureID);
	
	inline glm::vec3 GetColor() { return color_; 	}
	void SetColor(glm::vec3 color);

protected:
    glm::vec3 position_;
    glm::vec3 scale_;
    GLuint texture_;
    GLuint program_;
	glm::vec3 color_;
};

