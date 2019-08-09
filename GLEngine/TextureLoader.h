#pragma once
#include <gl/glew.h>
#include <string>

//jpg : rgb
//png : rgb+alpah
//dds : rgb+alpha
class TextureLoader
{
public:
	TextureLoader();
	~TextureLoader();

	GLuint GetTextureID(std::string textureFileName);	
};

