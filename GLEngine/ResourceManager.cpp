#include "pch.h"
#include "ResourceManager.h"
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <iostream>


ResourceManager::ResourceManager()
{
}


ResourceManager::~ResourceManager()
{
}

const struct aiScene* ResourceManager::LoadAsset(std::string path)
{
	auto it = assetCache_.find(path);
	if (it != assetCache_.end())
	{
		return it->second;
	}

	//�������� �ֻ��� �ɼ����� �����´�.
	//�������� �����ؼ� �����Ѱ�

	//aiProcess_Triangulate	//�ﰢ�� �ƴϸ� ���
	//aiProcess_FlipUVs	//opengl�� �ؽ��� v�� �ݴ�������, ������ �ֱ�
	//aiProcess_GenNormals	//�븻������ ����
	const aiScene* scene = aiImportFile(path.c_str(), aiProcessPreset_TargetRealtime_MaxQuality);
	assetCache_[path] = scene;

	if (!scene)
	{
		std::cout << aiGetErrorString() << std::endl;
	}	

	return scene;
}

