#pragma once
#include "glm/glm.hpp"
#include <vector>

enum MeshType
{
    Trangile = 0,
    Quad = 1,
    Cube = 2,
    Sphere = 3,	
};

struct Vertex
{
    glm::vec3 pos;
    glm::vec3 normal;
    glm::vec3 color;
    glm::vec2 texCoords;
};

class Mesh
{
public:
    Mesh();
    ~Mesh();

    static void setTriData(std::vector<Vertex>& vertices, std::vector<uint32_t>& indices);
    static void setQuadData(std::vector<Vertex>& vertices, std::vector<uint32_t>& indices);
    static void setCubeData(std::vector<Vertex>& vertices, std::vector<uint32_t>& indices);	
    static void setSphereData(std::vector<Vertex>& vertices, std::vector<uint32_t>& indices);

	static void setMineCraftFaceUV(std::vector<Vertex>& vertices);
};