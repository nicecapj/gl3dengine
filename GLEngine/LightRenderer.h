#pragma once

#include <gl/glew.h>
#include "Dependencies/glm/glm.hpp"
#include "Dependencies/glm/gtc/type_ptr.hpp"

#include "Mesh.h"
#include "ShaderLoader.h"
#include "Camera.h"

class LightRenderer
{
public:
    LightRenderer(MeshType meshType, Camera* camera);
    ~LightRenderer();

    void Draw();

    void SetPosition(glm::vec3 position);
    void SetColor(glm::vec3 color);
    void SetProgram(GLuint program);

    glm::vec3 GetPosition();
    glm::vec3 GetColor();

private:
    Camera* camera_;

    std::vector<Vertex> vertices_;
    std::vector<GLuint> indicies_;
    glm::vec3 position_;
    glm::vec3 color_;

    GLuint vbo_;	//vertex buffer object. this is geometrical information, attributes such as position, normal, color, texture coordination. store on a per vertex base on GPU
    GLuint ebo_;	//element buffer object. this is used to store vertex index. such as index buffer of directX
    GLuint vao_;	//vertex array object. this is helper container object that store all VBOs and attributes. 오브젝트마다 1개이상의 VBO가 있을텐데, 각 프레임을 랜더링 할때마다, 바인드하긴 지루하니까 이걸 사용

    //buffer store in GPU. Modern GPU bandwidth is 600GB/s, Modern CPU bandwidth is 12GB/s
    //Buffer objects are used to store, retrive, move data.
    GLuint program_;
};

