#pragma once
#include <gl/glew.h>
#include <string>

//jpg : rgb
//png : rgb+alpah
//dds : rgb+alpha
class TextureLoader
{
	friend class TextureManager;

public:
	TextureLoader();
	~TextureLoader();
	
private:
	GLuint GetTextureID(std::string textureFileName);	
};

