#pragma once
#include <string>
#include <gl/glew.h>

class ShaderLoader
{
public:
    ShaderLoader();
    ~ShaderLoader();

    GLuint CreateProgram(const char* vertexShaderFilename, const char* fragmentShaderFile);
private:
    std::string ReadShader(const char* filename);
    GLuint CreateShader(GLenum shaderType, std::string source, const char* shaderNameForLog);

};

