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
	//if (getFileExt(textureFileName) != "dds")
	//{
	//	assert(0);
	//	return -1;
	//}

	auto it = textureCache_.find(textureFileName);
	if (it != textureCache_.end())
	{
		return it->second;
	}

//	GLuint textureId = loader_->GetTextureID(textureFileName, true);
	GLuint textureId = loader_->GetTextureID(textureFileName);
	textureCache_[textureFileName] = textureId;
	
	return textureId;
}

GLuint TextureManager::GetCubemapTextureID(std::string textureFileName, std::string ext)
{
	auto it = textureCache_.find(textureFileName);
	if (it != textureCache_.end())
	{
		return it->second;
	}

	std::string textures_faces[6];
	textures_faces[0] = (textureFileName + "_rt" + ext);	//GL_TEXTURE_CUBE_MAP_POSITIVE_X
	textures_faces[1] = (textureFileName + "_lf" + ext);	//GL_TEXTURE_CUBE_MAP_NEGATIVE_X
	textures_faces[2] = (textureFileName + "_up" + ext);	//GL_TEXTURE_CUBE_MAP_POSITIVE_Y
	textures_faces[3] = (textureFileName + "_dn" + ext);	//GL_TEXTURE_CUBE_MAP_NEGATIVE_Y
	textures_faces[4] = (textureFileName + "_bk" + ext);	//GL_TEXTURE_CUBE_MAP_POSITIVE_Z
	textures_faces[5] = (textureFileName + "_ft" + ext);	//GL_TEXTURE_CUBE_MAP_NEGATIVE_Z	

	GLuint textureId = loader_->GetCubemapTextureID(textures_faces);
	textureCache_[textureFileName] = textureId;
	return textureId;
}