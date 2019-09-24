#include "pch.h"
#include "ReflectionCube.h"
#include "Camera.h"


ReflectionCube::ReflectionCube(MeshType meshType, class Camera* camera)
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


ReflectionCube::~ReflectionCube()
{
}

void ReflectionCube::PreDraw()
{
    Renderer::PreDraw();
}

void ReflectionCube::Draw()
{
    glUseProgram(program_);

    //model ->  view -> projection matrix를 uniform variables를 통해 쉐이더로 정보를 보낼수 있다.
    //glGetUniformLocation()함수로 쉐이더프로그램 안에 선언된 변수의 이름으로 가져올 수 있다. ex) uniform mat4 model;
    GLuint modelLocation = glGetUniformLocation(program_, "model");
    //glUniformXXX형식의 함수로 쉐이더에  연결된 유니폼 변수에, 값을 설정할 수 있다.
    glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model_));	//유니폼변수, 넘길데이터의 수, 전치인지 여부, 넘길 DATA의 포인터

    glm::mat4 view = camera_->GetViewMatrix();
    glm::mat4 proj = camera_->GetProjectMatrix();
    glm::mat4 vp = proj * view;

    GLuint vpLocation = glGetUniformLocation(program_, "vp");	//uniform mat4 view;
    glUniformMatrix4fv(vpLocation, 1, GL_FALSE, glm::value_ptr(vp));


    auto camPos = camera_->GetCameraPosition();
    GLuint camPosLocation = glGetUniformLocation(program_, "cameraPos");	//uniform vec3 cameraPos;
    assert(camPosLocation != -1);
    glUniform3f(camPosLocation, camPos.x, camPos.y, camPos.z);

    //texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, GetTexture(0));


    //draw
    //한번만 그릴 데이터를 전부 요구한다.(vao_) 이후 glDrawElements를 통해 그린다.
    glBindVertexArray(vao_);
    glDrawElements(GL_TRIANGLES, (GLsizei)indicies_.size(), GL_UNSIGNED_INT, 0);

    //marks end of draw function
    glBindVertexArray(0);
    glUseProgram(0);


    Renderer::Draw();
}

void ReflectionCube::PostDraw()
{
    throw std::logic_error("The method or operation is not implemented.");
}
