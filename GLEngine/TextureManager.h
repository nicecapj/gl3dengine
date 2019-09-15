#pragma once

//support to loading dds format image.
#include <gl/glew.h>
#include<map>

class TextureManager
{
public:	
	static TextureManager* GetInstance() {
		return &instance_;
	}
	TextureManager();
	~TextureManager();

	GLuint GetTextureID(std::string textureFileName);

private:	
	std::string getFileExt(const std::string& s);	//todo :: must move to utility file.
		
	std::map<std::string, GLuint> textureCache_;

	static TextureManager instance_;
	class TextureLoader* loader_;
};

