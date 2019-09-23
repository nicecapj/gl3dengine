#include "pch.h"
#include "Mesh.h"


Mesh::Mesh()
{
}


Mesh::~Mesh()
{
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
