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

	//느리지만 최상의 옵션으로 가져온다.
	//수동으로 설정해서 쓸만한건

	//aiProcess_Triangulate	//삼각형 아니면 경고
	//aiProcess_FlipUVs	//opengl은 텍스쳐 v가 반대임으로, 뒤집어 주기
	//aiProcess_GenNormals	//노말없으면 생성
	const aiScene* scene = aiImportFile(path.c_str(), aiProcessPreset_TargetRealtime_MaxQuality);
	assetCache_[path] = scene;

	if (!scene)
	{
		std::cout << aiGetErrorString() << std::endl;
	}	

	return scene;
}

