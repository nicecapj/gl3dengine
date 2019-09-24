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


    //Vertex���� �ɹ��� ������ �߿����� �ʴ�. ������ �ε����� ���̴� �ȿ��� layout (location = 0)�� ��Ȯ�ϰ�
    //����Ű�� ���� �߿��ϴ�. �߰��� �ɹ��� �ǳʶٰų� �����ص� �������.
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


    auto camPos = camera_->GetCameraPosition();
    GLuint camPosLocation = glGetUniformLocation(program_, "cameraPos");	//uniform vec3 cameraPos;
    assert(camPosLocation != -1);
    glUniform3f(camPosLocation, camPos.x, camPos.y, camPos.z);

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

void ReflectionCube::PostDraw()
{
    throw std::logic_error("The method or operation is not implemented.");
}
