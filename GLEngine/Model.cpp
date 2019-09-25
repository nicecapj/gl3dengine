#include "pch.h"
#include "Model.h"
#include "ResourceManager.h"
#include "TextureManager.h"
#include "assimp/scene.h"
#include "assimp/mesh.h"

#include "assimp/cimport.h"
#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include <iostream>

void Model::Draw(GLuint shader)
{
	for (unsigned int i = 0; i < meshes_.size(); i++)
		meshes_[i].Draw(shader);
}

Model::~Model()
{
}

void Model::LoadModel(std::string path)
{
	//const auto* scene = ResourceManager::GetInstance()->LoadAsset(path);
	//if (!scene)
	//	return;
	Assimp::Importer import;
	const aiScene *scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
		return;
	}
	directory = path.substr(0, path.find_last_of('/'));

	ProcessNode(scene->mRootNode, scene);
}

void Model::ProcessNode(struct aiNode *node, const struct aiScene *scene)
{
	//가지고 있는 모든 메쉬처리후
	for (int i = 0; i < node->mNumMeshes; ++i)
	{
		auto mesh = scene->mMeshes[node->mMeshes[i]];
		meshes_.push_back(ProcessMesh(mesh, scene));
	}

	//자식들도 재귀
	for (int i = 0; i < node->mNumChildren; ++i)
	{
		ProcessNode(node->mChildren[i], scene);
	}
}

Mesh Model::ProcessMesh(struct aiMesh *mesh, const struct aiScene *scene)
{
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	std::vector<Texture> textures;


	for (int i = 0; i < mesh->mNumVertices; ++i)
	{
		Vertex vertex;

		glm::vec3 vector;
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex.pos = vector;

		vector.x = mesh->mNormals[i].x;
		vector.y = mesh->mNormals[i].y;
		vector.z = mesh->mNormals[i].z;
		vertex.normal = vector;

		if (mesh->mTextureCoords[0]) // mesh가 텍스처 좌표를 가지고 있는가?
		{
			glm::vec2 vec;
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.texCoords = vec;
		}
		else
		{
			vertex.texCoords = glm::vec2(0.0f, 0.0f);
		}		

		vertices.push_back(vertex);
	}

	for (GLuint i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (GLuint j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
		std::vector<Texture> diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		std::vector<Texture> specularMaps = LoadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	}
	
	return Mesh(vertices, indices, textures);
}

std::vector<Texture> Model::LoadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName)
{
	std::vector<Texture> textures;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		Texture texture;
		texture.Id = TextureManager::GetInstance()->GetTextureID(str.C_Str());
		texture.TextureType = typeName;
		//texture.Path = str.C_Str();
		textures.push_back(texture);
	}
	return textures;
}

