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

	~Model();

private:
	void LoadModel(std::string path);
	void ProcessNode(struct aiNode *node, const struct aiScene *scene);
	Mesh ProcessMesh(struct aiMesh *mesh, const struct aiScene *scene);
	std::vector<Texture> LoadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
	
	std::vector<Mesh> meshes_;
	std::string directory;
};

