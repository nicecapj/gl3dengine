#include "pch.h"
#include "ShaderManager.h"
#include <iostream>
#include <fstream>
#include <vector>

ShaderManager::ShaderManager()
{

}

ShaderManager::~ShaderManager()
{
	vertexShaderCache_.clear();
	fragmentShaderCache_.clear();
	programCache_.clear();
}

GLuint ShaderManager::GetProgram(const char* vertexShaderFilename, const char* fragmentShaderFile)
{
	GLuint vsId = GL_FALSE;
	auto it = vertexShaderCache_.find(vertexShaderFilename);
	if (it != vertexShaderCache_.end())
	{
		vsId = it->second;
	}

	GLuint fsId = GL_FALSE;
	auto fit = fragmentShaderCache_.find(fragmentShaderFile);
	if (fit != fragmentShaderCache_.end())
	{
		fsId = fit->second;
	}

	GLuint program = GL_FALSE;
	if (vsId && fsId)
	{
		int key = vsId << 20;
		key |= fsId;

		auto pit = programCache_.find(key);
		if (pit != programCache_.end())
		{
			return pit->second;
		}		
	}

	if (vsId == GL_FALSE)
	{
		auto vsFile = ReadShader(vertexShaderFilename);
		vsId = CreateShader(GL_VERTEX_SHADER, vsFile, "vertexShader");
		vertexShaderCache_[vertexShaderFilename] = vsId;
		if (vsId == GL_FALSE)
		{ 
			return GL_FALSE;
		}		
	}

	if (fsId == GL_FALSE)
	{
		auto fsFile = ReadShader(fragmentShaderFile);
		fsId = CreateShader(GL_FRAGMENT_SHADER, fsFile, "fragmentShader");
		fragmentShaderCache_[fragmentShaderFile] = fsId;
		if (fsId == GL_FALSE)
		{
			return GL_FALSE;
		}
	}
	

	if (program == GL_FALSE)
	{
		program = glCreateProgram();
		glAttachShader(program, vsId);
		glAttachShader(program, fsId);
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
			std::cout << "ERROR Link shader program : " << vertexShaderFilename << " " << fragmentShaderFile << std::endl << link_log.data() << std::endl;
			return GL_FALSE;
		}

		int key = vsId << 20;		
		key |= fsId;

		programCache_[key] = program;
		return program;
	}

	return GL_FALSE;
}

std::string ShaderManager::ReadShader(const char* filename)
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
GLuint ShaderManager::CreateShader(GLenum shaderType, std::string source, const char* shaderNameForLog)
{
	int compileResult = 0;
	GLuint shader = glCreateShader(shaderType);
	const char* shaderCodePtr = source.c_str();
	const int shaderCodeSize = (int)source.size();

	glShaderSource(shader, 1, &shaderCodePtr, &shaderCodeSize);
	glCompileShader(shader);

	glGetShaderiv(shader, GL_COMPILE_STATUS, &compileResult);
	if (compileResult == GL_FALSE)
	{
		int logLenth = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLenth);
		std::vector<char> shader_log(logLenth);
		glGetShaderInfoLog(shader, logLenth, nullptr, &shader_log[0]);
		std::cout << "ERROR Compile shader : " << shaderNameForLog << std::endl << shader_log.data() << std::endl;
		return 0;
	}
	return shader;
}