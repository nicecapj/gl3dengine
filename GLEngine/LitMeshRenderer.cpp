#include "pch.h"
#include "Camera.h"
#include "LitMeshRenderer.h"
#include "LightRenderer.h"


LitMeshRenderer::LitMeshRenderer(MeshType meshType, Camera* camera, class LightRenderer* light)
    :Renderer()
{
    light_ = light;
    camera_ = camera;

    switch (meshType)
    {
        case MeshType::Trangile:
            {
                Mesh::setTriData(vertices_, indices_);
            } break;
        case MeshType::Cube:
            {
                Mesh::setCubeData(vertices_, indices_);
            } break;
        case MeshType::Sphere:
            {
                Mesh::setSphereData(vertices_, indices_);
            } break;
        case MeshType::Quad:
            {
                Mesh::setQuadData(vertices_, indices_);
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

    //glEnableVertexAttribArray(3);	//VS : layout (location = 3) in vec3 color;
    //glVertexAttribPointer(3,
    //                      3,
    //                      GL_FLOAT,
    //                      GL_FALSE,
    //                      sizeof(Vertex),
    //                      (GLvoid*)(offsetof(Vertex, Vertex::color)));

    //EBO
    glGenBuffers(1, &ebo_);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indices_.size(), &indices_[0], GL_STATIC_DRAW);

    //Unbind buffer and vertex array as a precaution
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}


LitMeshRenderer::~LitMeshRenderer()
{
}

void LitMeshRenderer::Draw()
{
 //   glm::mat4 transformMatrix = glm::translate(glm::mat4(1.0), position_);
 //   glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0), scale_);    
	//glm::mat4 model = transformMatrix * matRot_ * scaleMatrix;

    //shader
    glUseProgram(program_);

    GLuint modelLocation = glGetUniformLocation(program_, "model");
    glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model_));	//유니폼변수, 넘길데이터의 수, 전치인지 여부, 넘길 DATA의 포인터

    glm::mat4 view = camera_->GetViewMatrix();
    glm::mat4 proj = camera_->GetProjectMatrix();
    glm::mat4 vp = proj * view;

    //uniform은 쉐이더 에러나, 쉐이더에서 선언만 되고 사용되는 곳 없으면, 가져오는데 실패(-1) 한다.
    GLuint vpLocation = glGetUniformLocation(program_, "vp");	//uniform mat4 view;
    //assert(vpLocation != -1);
    glUniformMatrix4fv(vpLocation, 1, GL_FALSE, glm::value_ptr(vp));

    auto camPos = camera_->GetCameraPosition();
    GLuint camPosLocation = glGetUniformLocation(program_, "cameraPos");	//uniform vec3 cameraPos;
    assert(camPosLocation != -1);
    glUniform3f(camPosLocation, camPos.x, camPos.y, camPos.z);

    GLuint lightPosLocation = glGetUniformLocation(program_, "lightPos");	//uniform vec3 lightPos;
    //assert(lightPosLocation != -1);
    auto lightPos = light_->GetPosition();
    glUniform3f(lightPosLocation, lightPos.x, lightPos.y, lightPos.z);

    //texture
    glBindTexture(GL_TEXTURE_2D, GetTexture(0));

    //draw
    //한번만 그릴 데이터를 전부 요구한다.(vao_) 이후 glDrawElements를 통해 그린다.
    glBindVertexArray(vao_);
    glDrawElements(GL_TRIANGLES, (GLsizei)indices_.size(), GL_UNSIGNED_INT, 0);

    //marks end of draw function
    glBindVertexArray(0);
    glUseProgram(0);

	//Renderer::Draw();
}

void LitMeshRenderer::PreDraw()
{
	Renderer::PreDraw();
}

void LitMeshRenderer::PostDraw()
{
	Renderer::PostDraw();
}
