#pragma once
#include "Singleton.h"
#include <gl/glew.h>
#include <map>

class ShaderManager : public Singleton< ShaderManager>
{
public:
	ShaderManager();
	~ShaderManager();

	GLuint GetProgram(const char* vertexShaderFilename, const char* fragmentShaderFile);
	
private:
	//GLuint CreateProgram(const char* vertexShaderFilename, const char* fragmentShaderFile);

	std::string ReadShader(const char* filename);
	GLuint CreateShader(GLenum shaderType, std::string source, const char* shaderNameForLog);

	std::map<std::string, GLuint> vertexShaderCache_;
	std::map<std::string, GLuint> fragmentShaderCache_;
	std::map<int, GLuint> programCache_;
};

