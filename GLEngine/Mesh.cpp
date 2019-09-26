#include "pch.h"
#include "Mesh.h"

#include <gl/glew.h>
#include "Dependencies/glm/glm.hpp"
#include "Dependencies/glm/gtc/matrix_transform.hpp"
#include "Dependencies/glm/gtc/type_ptr.hpp"

#include "Camera.h"
#include "LightRenderer.h"
#include "TextureManager.h"


Mesh::Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<Texture> textures)
	:vertices_(vertices), indices_(indices), textures_(textures)
{
	SetupMesh();

	SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	SetScale(glm::vec3(1.0f, 1.0f, 1.0f));
	SetRotation(glm::vec3(0.0f, 0.0f, 0.0f));

	envTextureForTest_ = TextureManager::GetInstance()->GetCubemapTextureID("Assets/Textures/skybox/mountain", ".tga");
}

Mesh::~Mesh()
{
}

void Mesh::Draw(GLuint shader)
{

	glm::mat4 transformMatrix = glm::translate(glm::mat4(1.0), position_);
	glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0), scale_);	
	model_ = scaleMatrix * matRot_ * transformMatrix;	//gl은 열우선행렬임으로 SRT

	glUseProgram(shader);

	GLuint modelLocation = glGetUniformLocation(shader, "model");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model_));	//유니폼변수, 넘길데이터의 수, 전치인지 여부, 넘길 DATA의 포인터

	glm::mat4 view = camera_->GetViewMatrix();
	glm::mat4 proj = camera_->GetProjectMatrix();
	glm::mat4 vp = proj * view;

	//uniform은 쉐이더 에러나, 쉐이더에서 선언만 되고 사용되는 곳 없으면, 가져오는데 실패(-1) 한다.
	GLuint vpLocation = glGetUniformLocation(shader, "vp");	//uniform mat4 view;
	//assert(vpLocation != -1);
	glUniformMatrix4fv(vpLocation, 1, GL_FALSE, glm::value_ptr(vp));

	auto camPos = camera_->GetCameraPosition();
	GLuint camPosLocation = glGetUniformLocation(shader, "cameraPos");	//uniform vec3 cameraPos;
	assert(camPosLocation != -1);
	glUniform3f(camPosLocation, camPos.x, camPos.y, camPos.z);

	GLuint lightPosLocation = glGetUniformLocation(shader, "lightPos");	//uniform vec3 lightPos;
	//assert(lightPosLocation != -1);
	auto lightPos = light_->GetPosition();
	glUniform3f(lightPosLocation, lightPos.x, lightPos.y, lightPos.z);


	//텍스쳐를 여러개 지원하기 위한 구조. 정해진 이름으로 사용
	//uniform sampler2D texture_diffuse1;
	//uniform sampler2D texture_diffuse2;
	//uniform sampler2D texture_diffuse3;
	//uniform sampler2D texture_specular1;
	//uniform sampler2D texture_specular2;
	//uniform sampler2D texture_normal1;
	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;
	unsigned int normalNr = 1;
	unsigned int i = 0;
	for (i = 0; i < textures_.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);

		std::string number;
		std::string name = textures_[i].TextureType;
		if (name == "texture_diffuse")
			number = std::to_string(diffuseNr++);
		else if (name == "texture_specular")
			number = std::to_string(specularNr++);
		else if (name == "texture_normal")
			number = std::to_string(normalNr++);

		GLuint shadowMapLocation = glGetUniformLocation(shader, (name + number).c_str());
		glUniform1i(shadowMapLocation, i);
		glBindTexture(GL_TEXTURE_2D, textures_[i].Id);
	}

	glActiveTexture(GL_TEXTURE0 + i);
	glBindTexture(GL_TEXTURE_CUBE_MAP, envTextureForTest_);

	glActiveTexture(GL_TEXTURE0);

	// mesh 그리기
	glBindVertexArray(vao_);
	glDrawElements(GL_TRIANGLES, (GLsizei)indices_.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Mesh::SetupMesh()
{
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

// Unity coord
//        +y
//         | z
//         | /
//    -x ---/------- +x
//         /|
//      -z/ | -y

// opengl coord
//        +y
//         | -z
//         | /
//    -x ---/------- +x
//         /|
//      +z/ | -y
void Mesh::setTriData(std::vector<Vertex>& vertices, std::vector<uint32_t>& indices)
{
    //            /\(0,-1)
    //           /  \
    //   (-1,-1)/----\ (1,1)
    //
    std::vector< Vertex> vertices_ =
    {
        {
            {0.0f, -1.0f, 0.0f},	//pos
            {0.0f, 0.0f, 1.0f},	//normal
            {0.0f, 1.0f, 0.0f},	//color
            {0.0, 1.0},	//tex
        },
        {
            {1.0f, 1.0f, 0.0f},	//pos
            {0.0f, 0.0f, 1.0f},	//normal
            {1.0f, 0.0f, 0.0f},	//color
            {0.0, 1.0},	//tex
        },
        {
            {-1.0f, 1.0f, 0.0f},	//pos
            {0.0f, 0.0f, 1.0f},	//normal
            {0.0f, 0.0f, 1.0f},	//color
            {0.0, 1.0},	//tex
        },
    };

    std::vector<uint32_t> indices_ = { 0,1,2 };

    vertices.clear();
    indices.clear();

    vertices = vertices_;
    indices = indices_;
}

void Mesh::setQuadData(std::vector<Vertex>& vertices, std::vector<uint32_t>& indices)
{
    std::vector<Vertex> _vertices =
    {

        { { -1.0f, -1.0f, 0.0f },{ 0.0f, 0.0f, 1.0 },{ 1.0f, 0.0f, 0.0 },{ 0.0, 1.0 } },
        { { -1.0f, 1.0f, 0.0f },{ 0.0f, 0.0f, 1.0 },{ 0.0f, 1.0f, 0.0 },{ 0.0, 0.0 } },
        { { 1.0f, 1.0f, 0.0f },{ 0.0f, 0.0f, 1.0 },{ 0.0f, 0.0f, 1.0 },{ 1.0, 0.0 } },
        { { 1.0f, -1.0f, 0.0f },{ 0.0f, 0.0f, 1.0 },{ 1.0f, 0.0f, 1.0 },{ 1.0, 1.0 } }
    };


    std::vector<uint32_t> _indices =
    {

        0, 1, 2,
        0, 2, 3
    };

    vertices.clear(); indices.clear();
    vertices = _vertices;
    indices = _indices;
}

void Mesh::setCubeData(std::vector<Vertex>& vertices, std::vector<uint32_t>& indices)
{
    std::vector<Vertex> _vertices =
    {

        //front
        { { -1.0f, -1.0f, 1.0f },{ 0.0f, 0.0f, 1.0 },{ 1.0f, 0.0f, 0.0 },{ 0.0, 1.0 } },
        { { -1.0f, 1.0f, 1.0f },{ 0.0f, 0.0f, 1.0 },{ 0.0f, 1.0f, 0.0 },{ 0.0, 0.0 } },
        { { 1.0f, 1.0f, 1.0f },{ 0.0f, 0.0f, 1.0 },{ 0.0f, 0.0f, 1.0 },{ 1.0, 0.0 } },
        { { 1.0f, -1.0f, 1.0f },{ 0.0f, 0.0f, 1.0 },{ 1.0f, 0.0f, 1.0 },{ 1.0, 1.0 } },

        // back
        { { 1.0, -1.0, -1.0 },{ 0.0f, 0.0f, -1.0 },{ 1.0f, 0.0f, 1.0 },{ 0.0, 1.0 } }, //4
        { { 1.0f, 1.0, -1.0 },{ 0.0f, 0.0f, -1.0 },{ 0.0f, 1.0f, 1.0 },{ 0.0, 0.0 } }, //5
        { { -1.0, 1.0, -1.0 },{ 0.0f, 0.0f, -1.0 },{ 0.0f, 1.0f, 1.0 },{ 1.0, 0.0 } }, //6
        { { -1.0, -1.0, -1.0 },{ 0.0f, 0.0f, -1.0 },{ 1.0f, 0.0f, 1.0 },{ 1.0, 1.0 } }, //7


        //left
        { { -1.0, -1.0, -1.0 },{ -1.0f, 0.0f, 0.0 },{ 0.0f, 0.0f, 1.0 },{ 0.0, 1.0 } }, //8
        { { -1.0f, 1.0, -1.0 },{ -1.0f, 0.0f, 0.0 },{ 0.0f, 0.0f, 1.0 },{ 0.0, 0.0 } }, //9
        { { -1.0, 1.0, 1.0 },{ -1.0f, 0.0f, 0.0 },{ 0.0f, 0.0f, 1.0 },{ 1.0, 0.0 } },   //10
        { { -1.0, -1.0, 1.0 },{ -1.0f, 0.0f, 0.0 },{ 0.0f, 0.0f, 1.0 },{ 1.0, 1.0 } }, //11

        //right
        { { 1.0, -1.0, 1.0 },{ 1.0f, 0.0f, 0.0 },{ 0.0f, 0.0f, 1.0 },{ 0.0, 1.0 } }, // 12
        { { 1.0f, 1.0, 1.0 },{ 1.0f, 0.0f, 0.0 },{ 0.0f, 0.0f, 1.0 },{ 0.0, 0.0 } }, //13
        { { 1.0, 1.0, -1.0 },{ 1.0f, 0.0f, 0.0 },{ 0.0f, 0.0f, 1.0 },{ 1.0, 0.0 } }, //14
        { { 1.0, -1.0, -1.0 },{ 1.0f, 0.0f, 0.0 },{ 0.0f, 0.0f, 1.0 },{ 1.0, 1.0 } }, //15

        //top
        { { -1.0f, 1.0f, 1.0f },{ 0.0f, 1.0f, 0.0 },{ 0.0f, 0.0f, 1.0 },{ 0.0, 1.0 } }, //16
        { { -1.0f, 1.0f, -1.0f },{ 0.0f, 1.0f, 0.0 },{ 0.0f, 0.0f, 1.0 },{ 0.0, 0.0 } }, //17
        { { 1.0f, 1.0f, -1.0f },{ 0.0f, 1.0f, 0.0 },{ 0.0f, 0.0f, 1.0 },{ 1.0, 0.0 } }, //18
        { { 1.0f, 1.0f, 1.0f },{ 0.0f, 1.0f, 0.0 },{ 0.0f, 0.0f, 1.0 },{ 1.0, 1.0 } }, //19

        //bottom
        { { -1.0f, -1.0, -1.0 },{ 0.0f, -1.0f, 0.0 },{ 0.0f, 0.0f, 1.0 },{ 0.0, 1.0 } }, //20
        { { -1.0, -1.0, 1.0 },{ 0.0f, -1.0f, 0.0 },{ 0.0f, 0.0f, 1.0 },{ 0.0, 0.0 } }, //21
        { { 1.0, -1.0, 1.0 },{ 0.0f, -1.0f, 0.0 },{ 0.0f, 0.0f, 1.0 },{ 1.0, 0.0 } },  //22
        { { 1.0, -1.0, -1.0 },{ 0.0f, -1.0f, 0.0 },{ 0.0f, 0.0f, 1.0 },{ 1.0, 1.0 } }, //23

    };

    std::vector<uint32_t> _indices =
    {

        0, 1, 2,
        2, 3, 0,

        4, 5, 6,
        4, 6, 7,

        8, 9, 10,
        8, 10, 11,

        12, 13, 14,
        12, 14, 15,

        16, 17, 18,
        16, 18, 19,

        20, 21, 22,
        20, 22, 23

    };


    vertices.clear(); indices.clear();
    vertices = _vertices;
    indices = _indices;
}

//minecraft 캐릭터 텍스쳐는 64x32사이즈로, 8 x 4개의 블록으로 구성되 있다.
//dds는 v가 뒤집혀있다. 일단 고려안하고, 이미지 뒤집어서 dds로 저장했다.
void Mesh::setMineCraftFaceUV(std::vector<Vertex>& vertices)
{
	float u = 0.125; // 1 / 8;
	float v = 0.25;  // 1 / 4;
	
	float uvChangeFacotr = 1;

	//front
	vertices[0].texCoords = { u * 1, v * 2 };	//텍스쳐 좌표계가 V(Y)는 위로 올라갈수록 1이라서, 1-x
	vertices[1].texCoords = { u * 1, v * 1 };
	vertices[2].texCoords = { u * 2, v * 1 };
	vertices[3].texCoords = { u * 2, v * 2 };

	//back
	vertices[4].texCoords = { u * 3, v * 2 };
	vertices[5].texCoords = { u * 3, v * 1 };
	vertices[6].texCoords = { u * 4, v * 1 };
	vertices[7].texCoords = { u * 4, v * 2 };

	//left
	vertices[8].texCoords = { u * 0,  v * 2 };
	vertices[9].texCoords = { u * 0,  v * 1 };
	vertices[10].texCoords = { u * 1, v * 1 };
	vertices[11].texCoords = { u * 1, v * 2 };

	//right
	vertices[12].texCoords = { u * 2,  v * 2 };
	vertices[13].texCoords = { u * 2,  v * 1 };
	vertices[14].texCoords = { u * 3, v * 1 };
	vertices[15].texCoords = { u * 3, v * 2 };
	
	//top
	vertices[16].texCoords = { u * 1, v * 0 };
	vertices[17].texCoords = { u * 1, v * 1 };
	vertices[18].texCoords = { u * 2, v * 1 };
	vertices[19].texCoords = { u * 2, v * 0 };

	//bottom
	vertices[20].texCoords = { u * 2, v * 1 };
	vertices[21].texCoords = { u * 2, v * 0 };
	vertices[22].texCoords = { u * 3, v * 0 };
	vertices[23].texCoords = { u * 3, v * 1 };
}

void Mesh::setMineCraftBodyUV(std::vector <Vertex>& vertices)
{
	float u = 0.125; // 1 / 8;
	float v = 0.25;  // 1 / 4;

	float uvChangeFacotr = 1;

	//front
	vertices[0].texCoords = { u * 4.0, v * 4.0 };	//텍스쳐 좌표계가 V(Y)는 위로 올라갈수록 1이라서, 1-x
	vertices[1].texCoords = { u * 4.0, v * 2.5 };
	vertices[2].texCoords = { u * 5.0, v * 2.5 };
	vertices[3].texCoords = { u * 5.0, v * 4.0 };

	//back
	vertices[4].texCoords = { u * 3.5, v * 4.0 };
	vertices[5].texCoords = { u * 3.5, v * 2.5 };
	vertices[6].texCoords = { u * 4.0, v * 2.5 };
	vertices[7].texCoords = { u * 4.0, v * 4.0 };

	//left
	vertices[8].texCoords = { u * 2,  v * 4.0 };
	vertices[9].texCoords = { u * 2,  v * 2.5 };
	vertices[10].texCoords = { u * 2.5, v * 2.5 };
	vertices[11].texCoords = { u * 2.5, v * 4.0 };

	//right
	vertices[12].texCoords = { u * 3.5,  v * 4.0 };
	vertices[13].texCoords = { u * 3.5,  v * 2.5 };
	vertices[14].texCoords = { u * 4.0,  v * 2.5 };
	vertices[15].texCoords = { u * 4.0,  v * 4.0 };

	//top
	vertices[16].texCoords = { u * 2.5, v * 2 };
	vertices[17].texCoords = { u * 2.5, v * 2.5 };
	vertices[18].texCoords = { u * 3.5, v * 2.5 };
	vertices[19].texCoords = { u * 3.5, v * 2 };

	//bottom
	vertices[20].texCoords = { u * 3.5, v * 2 };
	vertices[21].texCoords = { u * 3.5, v * 2.5 };
	vertices[22].texCoords = { u * 4.5, v * 2.5 };
	vertices[23].texCoords = { u * 4.5, v * 2 };
}

void Mesh::setMineCraftLeftArmUV(std::vector <Vertex>& vertices)
{
	float u = 0.125; // 1 / 8;
	float v = 0.25;  // 1 / 4;

	float uvChangeFacotr = 1;

	//front
	vertices[0].texCoords = { u * 5.5, v * 4.0 };	//텍스쳐 좌표계가 V(Y)는 위로 올라갈수록 1이라서, 1-x
	vertices[1].texCoords = { u * 5.5, v * 2.5 };
	vertices[2].texCoords = { u * 6.0, v * 2.5 };
	vertices[3].texCoords = { u * 6.0, v * 4.0 };

	//back
	vertices[4].texCoords = { u * 6, v * 4.0 };
	vertices[5].texCoords = { u * 6, v * 2.5 };
	vertices[6].texCoords = { u * 6.5, v * 2.5 };
	vertices[7].texCoords = { u * 6.5, v * 4.0 };

	//left
	vertices[8].texCoords = { u * 5,  v * 4.0 };
	vertices[9].texCoords = { u * 5,  v * 2.5 };
	vertices[10].texCoords = { u * 5.5, v * 2.5 };
	vertices[11].texCoords = { u * 5.5, v * 4.0 };

	//right
	vertices[12].texCoords = { u * 6.5,  v * 4.0 };
	vertices[13].texCoords = { u * 6.5,  v * 2.5 };
	vertices[14].texCoords = { u * 7.0,  v * 2.5 };
	vertices[15].texCoords = { u * 7.0,  v * 4.0 };

	//top
	vertices[16].texCoords = { u * 6.5, v * 2.5 };
	vertices[17].texCoords = { u * 6.5, v * 2.0 };
	vertices[18].texCoords = { u * 7.0, v * 2.0 };
	vertices[19].texCoords = { u * 7.0, v * 2.0 };

	//bottom
	vertices[20].texCoords = { u * 6.0, v * 2.5 };
	vertices[21].texCoords = { u * 6.0, v * 2.0 };
	vertices[22].texCoords = { u * 6.5, v * 2.0 };
	vertices[23].texCoords = { u * 6.5, v * 2.0 };
}

void Mesh::setMineCraftRightArmUV(std::vector <Vertex>& vertices)
{
	setMineCraftLeftArmUV(vertices);
}

void Mesh::setMineCraftLegUV(std::vector <Vertex>& vertices)
{
	float u = 0.125; // 1 / 8;
	float v = 0.25;  // 1 / 4;

	float uvChangeFacotr = 1;

	//front
	vertices[0].texCoords = { u * 0.0, v * 4.0 };	//텍스쳐 좌표계가 V(Y)는 위로 올라갈수록 1이라서, 1-x
	vertices[1].texCoords = { u * 0.0, v * 2.5 };
	vertices[2].texCoords = { u * 0.5, v * 2.5 };
	vertices[3].texCoords = { u * 0.5, v * 4.0 };

	//back
	vertices[4].texCoords = { u * 0.5, v * 4.0 };
	vertices[5].texCoords = { u * 0.5, v * 2.5 };
	vertices[6].texCoords = { u * 1.0, v * 2.5 };
	vertices[7].texCoords = { u * 1.0, v * 4.0 };

	//left
	vertices[8].texCoords = { u * 0,  v * 4.0 };
	vertices[9].texCoords = { u * 0,  v * 2.5 };
	vertices[10].texCoords = { u * 0.5, v * 2.5 };
	vertices[11].texCoords = { u * 0.5, v * 4.0 };

	//right
	vertices[12].texCoords = { u * 1.5,  v * 4.0 };
	vertices[13].texCoords = { u * 1.5,  v * 2.5 };
	vertices[14].texCoords = { u * 2.0,  v * 2.5 };
	vertices[15].texCoords = { u * 2.0,  v * 4.0 };

	//top
	vertices[16].texCoords = { u * 1.5, v * 2.5 };
	vertices[17].texCoords = { u * 1.5, v * 2.0 };
	vertices[18].texCoords = { u * 2.0, v * 2.0 };
	vertices[19].texCoords = { u * 2.0, v * 2.0 };

	//bottom
	vertices[20].texCoords = { u * 1.0, v * 2.5 };
	vertices[21].texCoords = { u * 1.0, v * 2.0 };
	vertices[22].texCoords = { u * 1.5, v * 2.0 };
	vertices[23].texCoords = { u * 1.5, v * 2.0 };
}

void Mesh::setSphereData(std::vector<Vertex>& vertices, std::vector<uint32_t>& indices)
{
    std::vector<Vertex> _vertices;
    std::vector<uint32_t> _indices;

    float latitudeBands = 20.0f;
    float longitudeBands = 20.0f;
    float radius = 1.0f;

    for (float latNumber = 0; latNumber <= latitudeBands; latNumber++)
    {
        float theta = latNumber * 3.14 / latitudeBands;
        float sinTheta = sin(theta);
        float cosTheta = cos(theta);

        for (float longNumber = 0; longNumber <= longitudeBands; longNumber++)
        {

            float phi = longNumber * 2 * 3.147 / longitudeBands;
            float sinPhi = sin(phi);
            float cosPhi = cos(phi);

            Vertex vs;

            vs.texCoords.x = (longNumber / longitudeBands); // u
            vs.texCoords.y = (latNumber / latitudeBands);   // v

            vs.normal.x = cosPhi * sinTheta;   // normal x
            vs.normal.y = cosTheta;            // normal y
            vs.normal.z = sinPhi * sinTheta;   // normal z

            vs.color.r = vs.normal.x;
            vs.color.g = vs.normal.y;
            vs.color.b = vs.normal.z;

            vs.pos.x = radius * vs.normal.x; // x
            vs.pos.y = radius * vs.normal.y; // y
            vs.pos.z = radius * vs.normal.z; // z

            _vertices.push_back(vs);
        }
    }

    for (uint32_t latNumber = 0; latNumber < latitudeBands; latNumber++)
    {
        for (uint32_t longNumber = 0; longNumber < longitudeBands; longNumber++)
        {
            uint32_t first = (latNumber * (longitudeBands + 1)) + longNumber;
            uint32_t second = first + longitudeBands + 1;

            _indices.push_back(first);
            _indices.push_back(second);
            _indices.push_back(first + 1);

            _indices.push_back(second);
            _indices.push_back(second + 1);
            _indices.push_back(first + 1);

        }
    }

    vertices.clear(); indices.clear();
    vertices = _vertices;
    indices = _indices;

}


void Mesh::SetCamera(class Camera* camera)
{
	camera_ = camera;
}

void Mesh::SetLight(class LightRenderer* light0)
{
	light_ = light0;
}

void Mesh::SetPosition(glm::vec3 position)
{
	position_ = position;
}

void Mesh::SetScale(glm::vec3 scale)
{
	scale_ = scale;
}

void Mesh::SetRotation(glm::vec3 bias, float angle)
{
	glm::quat q(bias * angle);
	matRot_ = glm::mat4_cast(q);
}

void Mesh::SetRotation(glm::vec3 eulerAngles)
{
	glm::quat q(eulerAngles);
	matRot_ = glm::mat4_cast(q);
}

glm::vec3 Mesh::GetRotationEuler()
{
	glm::quat q(matRot_);
	return glm::eulerAngles(q);
}