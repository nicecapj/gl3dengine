#pragma once
#include "glm/glm.hpp"
#include "gl/glew.h"
#include <vector>
#include <string>

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

struct Texture {
	GLuint Id;
	std::string TextureType;	
};

class Mesh
{
public:
    Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<Texture> textures);
    ~Mesh();

	void Draw(GLuint shader);
	void SetCamera(class Camera* camera);
	void SetLight(class LightRenderer* light0);	//우선 라이트는 1개만 지원

	//from Renderer
	inline glm::vec3 GetPosition() { return position_; }
	void SetPosition(glm::vec3 position);
	inline glm::vec3 GetScale() { return scale_; }
	void SetScale(glm::vec3 scale);
	void SetRotation(glm::vec3 bias, float angle);
	void SetRotation(glm::vec3 eulerAngles);
	const glm::mat4& GetRotation() { return matRot_; }
	glm::vec3 GetRotationEuler();


	//base primitive
    static void setTriData(std::vector<Vertex>& vertices, std::vector<uint32_t>& indices);
    static void setQuadData(std::vector<Vertex>& vertices, std::vector<uint32_t>& indices);
    static void setCubeData(std::vector<Vertex>& vertices, std::vector<uint32_t>& indices);	
    static void setSphereData(std::vector<Vertex>& vertices, std::vector<uint32_t>& indices);
	
	//test util
	static void setMineCraftFaceUV(std::vector<Vertex>& vertices);
	static void setMineCraftBodyUV(std::vector <Vertex>& vertices);
	static void setMineCraftLeftArmUV(std::vector <Vertex>& vertices);
	static void setMineCraftRightArmUV(std::vector <Vertex>& vertices);
	static void setMineCraftLegUV(std::vector <Vertex>& vertices);

private:
	void SetupMesh();

	std::vector<Vertex> vertices_;
	std::vector<GLuint> indices_;
	std::vector<Texture> textures_;

	GLuint vbo_;	//vertex buffer object. this is geometrical information, attributes such as position, normal, color, texture coordination. store on a per vertex base on GPU
	GLuint ebo_;	//element buffer object. this is used to store vertex index. such as index buffer of directX
	GLuint vao_;	//vertex array object. this is helper container object that store all VBOs and attributes. 오브젝트마다 1개이상의 VBO가 있을텐데, 각 프레임을 랜더링 할때마다, 바인드하긴 지루하니까 이걸 사용


	//from Renderer
	glm::vec3 position_;
	glm::vec3 scale_;
	glm::mat4 matRot_;
	glm::mat4 model_;

	class Camera* camera_;
	class LightRenderer* light_;
	
	GLuint envTexture_;
	GLuint sceneColorBuffer_;
};