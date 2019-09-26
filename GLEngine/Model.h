#pragma once
#include "Mesh.h"
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
	void Draw(GLuint shader);

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
	
	std::vector<Mesh> meshes_;	
};

