#include "pch.h"
#include "ShaderLoader.h"
#include <iostream>
#include <fstream>
#include <vector>


ShaderLoader::ShaderLoader()
{

}


ShaderLoader::~ShaderLoader()
{
}

GLuint ShaderLoader::CreateProgram(const char* vertexShaderFilename, const char* fragmentShaderFile)
{
    auto vsFile = ReadShader(vertexShaderFilename);
    auto fsFile = ReadShader(fragmentShaderFile);

    GLuint vs = CreateShader(GL_VERTEX_SHADER, vsFile, "vertexShader");
    GLuint fs = CreateShader(GL_FRAGMENT_SHADER, fsFile, "fragmentShader");

    //쉐이더 프로그램을 만들고, 거기에 버텍스쉐이더와 프래그먼트 쉐이더를 붙인다.
    GLuint program = glCreateProgram();
    glAttachShader(program, vs);
    glAttachShader(program, fs);

    //링크한다.
    glLinkProgram(program);

    int linkResult = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &linkResult);
    if (linkResult == GL_FALSE)
    {
        int logLenth = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLenth);
        std::vector<char> link_log(logLenth);
        glGetProgramInfoLog(program, logLenth, nullptr, &link_log[0]);
        std::cout << "ERROR Link shader program : " << std::endl << link_log[0] << std::endl;
        return 0;
    }

    return program;
}

std::string ShaderLoader::ReadShader(const char* filename)
{
    std::string shaderCode;
    std::ifstream file(filename, std::ios::in);
    if (!file.good())
    {
        std::cout << "Can`t read file " << filename << std::endl;
        std::terminate();
    }

    file.seekg(0, std::ios::end);
    shaderCode.resize((unsigned int)file.tellg());
    file.seekg(0, std::ios::beg);
    file.read(&shaderCode[0], shaderCode.size());
    file.close();

    return shaderCode;
}

/// shaderType : vertex or gragment shader
GLuint ShaderLoader::CreateShader(GLenum shaderType, std::string source, const char* shaderNameForLog)
{
    int compileResult = 0;
    GLuint shader = glCreateShader(shaderType);
    const char* shaderCodePtr = source.c_str();
    const int shaderCodeSize = source.size();

    glShaderSource(shader, 1, &shaderCodePtr, &shaderCodeSize);
    glCompileShader(shader);

    glGetShaderiv(shader, GL_COMPILE_STATUS, &compileResult);
    if (compileResult == GL_FALSE)
    {
        int logLenth = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLenth);
        std::vector<char> shader_log(logLenth);
        glGetShaderInfoLog(shader, logLenth, nullptr, &shader_log[0]);
        std::cout << "ERROR Compile shader : " << std::endl << shader_log[0] << shaderNameForLog << std::endl;
        return 0;
    }
    return shader;
}
