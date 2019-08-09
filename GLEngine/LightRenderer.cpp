#include "pch.h"
#include "LightRenderer.h"


LightRenderer::LightRenderer(MeshType meshType, Camera* camera)
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
    //버텍스어래이 오브젝트를 만들고, 어플리케이션에 바인드하면
    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);

    //버텍스버퍼 오브젝트를 만들어서 바인드 할수 있다. 메모리 형태로 관리됨으로, GL_ARRAY_BUFFER같은 타입 명시가 필요하다.
    glGenBuffers(1, &vbo_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);

    //버텍스버퍼 오브젝트가 만들어지면 실제 데이터를 넘겨서. 버퍼에 복사시킨다.
    //GL_STATIC_DRAW는 한번만 수정(설정)하고, 계속 사용하는 형식이다.
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices_.size(), &vertices_[0], GL_STATIC_DRAW);

    //position이나 normal등의 속성이 있지만, 모두 사용해야 하는 것은 아니다.
    //필요한 것은 position과 color만 필요함으로 이것만 활성화하자
    glEnableVertexAttribArray(0);	//Vertex 구조체 속성 0번 멤버 활성화 -> position으로 사용

    //normalize안된 값이라서 GL_FALSE
    glVertexAttribPointer(0,	//index of vertex array. 어플리케이션에 여러게 vao가 바인드되었을수 있음으로, 0번에 이 포인터 사용
                          3,	//x,y,z
                          GL_FLOAT,	//type of xyz
                          GL_FALSE,	//normalized. 0~1이 아닌 값이면 false한다
                          sizeof(Vertex),	//stride는 보폭이란 뜻으로, 버퍼에서 어느 사이즈로 뛰어가면서 읽을지를 결정하는 사이즈
                          (GLvoid*)0);	//버텍스 struct안에서 버텍스속성의 옵셋(position : 0, normal:1? 3? -> 12(pos:4x3))

    //debug
    //auto off = offsetof(Vertex, Vertex::color);

    glEnableVertexAttribArray(1);	//2번째(1번) 버텍스속성 활성화 -> color로 사용
    glVertexAttribPointer(1,	//index of vertex array. 어플리케이션에 여러게 vao가 바인드되었을수 있음으로, 1번에 이 포인터 사용
                          3,	//R,G,B
                          GL_FLOAT,	//type of RGB
                          GL_FALSE,	//normalized
                          sizeof(Vertex),	//stride는 보폭이란 뜻으로, 버퍼에서 어느 사이즈로 뛰어가면서 읽을지를 결정하는 사이즈
                          (GLvoid*)(offsetof(Vertex, Vertex::color)));	//옵셋을 알기 힘드니, offsetof를 사용하자.

    //EBO
    glGenBuffers(1, &ebo_);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indicies_.size(), &indicies_[0], GL_STATIC_DRAW);

    //Unbind buffer and vertex array as a precaution
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

LightRenderer::~LightRenderer()
{
}

void LightRenderer::Draw()
{
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(glm::mat4(1.0), position_);

    //set shader
    glUseProgram(program_);

    //model ->  view -> projection matrix를 uniform variables를 통해 쉐이더로 정보를 보낼수 있다.
    //glGetUniformLocation()함수로 쉐이더프로그램 안에 선언된 변수의 이름으로 가져올 수 있다. ex) uniform mat4 model;
    GLuint modelLocation = glGetUniformLocation(program_, "model");
    //glUniformXXX형식의 함수로 쉐이더에  연결된 유니폼 변수에, 값을 설정할 수 있다.
    glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));	//유니폼변수, 넘길데이터의 수, 전치인지 여부, 넘길 DATA의 포인터

    glm::mat4 view = camera_->GetViewMatrix();
    GLuint viewLocation = glGetUniformLocation(program_, "view");	//uniform mat4 view;
    glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));

    glm::mat4 proj = camera_->GetProjectMatrix();
    GLuint projectionLocation = glGetUniformLocation(program_, "projection");	////uniform mat4 projection;
    glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(proj));

    //draw
    //한번만 그릴 데이터를 전부 요구한다.(vao_) 이후 glDrawElements를 통해 그린다.
    glBindVertexArray(vao_);
    glDrawElements(GL_TRIANGLES, indicies_.size(), GL_UNSIGNED_INT, 0);

    //marks end of draw function
    glBindVertexArray(0);
    glUseProgram(0);
}

void LightRenderer::SetPosition(glm::vec3 position)
{
    position_ = position;
}

void LightRenderer::SetColor(glm::vec3 color)
{
    color_ = color;
}

void LightRenderer::SetProgram(GLuint program)
{
    program_ = program;
}

glm::vec3 LightRenderer::GetPosition()
{
    return position_;
}

glm::vec3 LightRenderer::GetColor()
{
    return color_;
}
