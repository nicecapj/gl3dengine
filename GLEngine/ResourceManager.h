#pragma once
#include "Singleton.h"
#include <string>
#include <map>

class ResourceManager :
	public Singleton< ResourceManager>
{
public:
	ResourceManager();
	virtual ~ResourceManager();

	const struct aiScene* LoadAsset(std::string path);

private:
	std::map<std::string, const struct aiScene*> assetCache_;
};

