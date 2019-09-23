#pragma once

//support to loading dds format image.
#include <gl/glew.h>
#include<map>
#include "Singleton.h"


// OPENGL TEXTURE COORD
// 
//  (0,1) --- (1,1)
//    |        |
//    |        |
//  (0,0) --- (1,0)
class TextureManager : public Singleton< TextureManager>
{
public:	
	TextureManager();
	~TextureManager();

	GLuint GetTextureID(std::string textureFileName);
	GLuint GetCubemapTextureID(std::string textureFileName, std::string ext);

private:	
	std::string getFileExt(const std::string& s);	//todo :: must move to utility file.
		
	std::map<std::string, GLuint> textureCache_;
	
	class TextureLoader* loader_;
};

