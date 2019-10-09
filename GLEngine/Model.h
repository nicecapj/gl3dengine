#pragma once
#include "Mesh.h"
#include "Animation.h"
#include <string>
#include <vector>
#include "assimp/material.h"

class Model
{
public:
	Model(std::string path)
	{
		LoadModel(path);
	}
	void PreDraw(GLuint shader);
	void Draw(GLuint shader);
	void PostDraw(GLuint shader);

	void SetCamera(class Camera* camera);
	void SetLight(class LightRenderer* light0);

	//from Renderer
	void SetPosition(glm::vec3 position);
	void SetScale(glm::vec3 scale);

	~Model();

private:
	void LoadModel(std::string path);
	void ProcessNode(struct aiNode *node, const struct aiScene *scene);
	Mesh ProcessMesh(struct aiMesh *mesh, const struct aiScene *scene);
	std::vector<Texture> LoadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
	
	std::string directroy_;	//텍스쳐는 절대경로가 아니라 mesh 모델 경로 필요함.
	std::vector<Mesh> meshes_;
	std::vector<Animation> animations_;

	glm::mat4 inverseTransform_;
};

