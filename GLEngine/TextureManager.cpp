#include "pch.h"
#include "TextureManager.h"
#include "TextureLoader.h"
#include <assert.h>

std::string TextureManager::getFileExt(const std::string& s)
{
	size_t i = s.rfind('.', s.length());
	if (i != std::string::npos) {
		return(s.substr(i + 1, s.length() - i));
	}

	return("");
}

TextureManager::TextureManager()
{
	loader_ = new TextureLoader();
}


TextureManager::~TextureManager()
{
	delete loader_;
	loader_ = nullptr;
}

GLuint TextureManager::GetTextureID(std::string textureFileName)
{
	if (getFileExt(textureFileName) != "dds")
	{
		assert(0);
		return -1;
	}

	auto it = textureCache_.find(textureFileName);
	if (it != textureCache_.end())
	{
		return it->second;
	}

	GLuint textureId = loader_->GetTextureID(textureFileName);
	textureCache_[textureFileName] = textureId;
	
	return textureId;
}