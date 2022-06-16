#include "graphics/Shader.h"

#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>

#include <glad/glad.h>

namespace Graphics
{

Shader::Shader(const std::string& vertexShaderPath, const std::string& fragShaderPath)
{
	std::string _vertexCode, _fragCode;
	std::ifstream _vertexShaderFile, _fragShaderFile;
	
	constexpr auto _fileExceptions = std::ifstream::failbit | std::ifstream::badbit;
	_vertexShaderFile.exceptions(_fileExceptions);
	_fragShaderFile.exceptions(_fileExceptions);
	
	try
	{
		_vertexShaderFile.open(vertexShaderPath);
		_fragShaderFile.open(fragShaderPath);
		
		std::stringstream _vertexShaderStream, _fragShaderStream;
		
		_vertexShaderStream << _vertexShaderFile.rdbuf();
		_fragShaderStream << _fragShaderFile.rdbuf();
		
		_vertexShaderFile.close();
		_fragShaderFile.close();
		
		_vertexCode = _vertexShaderStream.str();
		_fragCode = _fragShaderStream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "Error reading shader file: " << e.what();
	}
	
	const char* _cStyleVertexCode = _vertexCode.c_str();
	const char* _cStyleFragCode = _fragCode.c_str();
	
	unsigned int _vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	unsigned int _fragShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	int _shaderCompileStatus = 0;
	char _info[512] = {0};
	
	glShaderSource(_vertexShaderID, 1, &_cStyleVertexCode, nullptr);
	glCompileShader(_vertexShaderID);

	glGetShaderiv(_vertexShaderID, GL_COMPILE_STATUS, &_shaderCompileStatus);
	if (!_shaderCompileStatus)
	{
		std::memset(_info, 0, sizeof(_info));
		glGetShaderInfoLog(_vertexShaderID, 512, nullptr, _info);
		std::cout << "Error compiling vertex shader: " << _info;
		return;
	}
	
	glShaderSource(_fragShaderID, 1, &_cStyleFragCode, nullptr);
	glCompileShader(_fragShaderID);

	glGetShaderiv(_fragShaderID, GL_COMPILE_STATUS, &_shaderCompileStatus);
	if (!_shaderCompileStatus)
	{
		std::memset(_info, 0, sizeof(_info));
		glGetShaderInfoLog(_fragShaderID, 512, nullptr, _info);
		std::cout << "Error compiling fragment shader: " << _info;
		return;
	}
	
	unsigned int _shaderProgram = glCreateProgram();
	glAttachShader(_shaderProgram, _vertexShaderID);
	glAttachShader(_shaderProgram, _fragShaderID);
	glLinkProgram(_shaderProgram);

	glGetShaderiv(_shaderProgram, GL_LINK_STATUS, &_shaderCompileStatus);
	if (!_shaderCompileStatus)
	{
		std::memset(_info, 0, sizeof(_info));
		glGetProgramInfoLog(_shaderProgram, 512, nullptr, _info);
		std::cout << "Error linking shaders: " << _info;
		return;
	}
	
	programID = _shaderProgram;
	glDeleteShader(_vertexShaderID);
	glDeleteShader(_fragShaderID);
}

void Shader::Use()
{
	glUseProgram(programID);
}

void Shader::SetBool(const std::string& propertyName, bool value)
{
	glUniform1i(glGetUniformLocation(programID, propertyName.c_str()), static_cast<int>(value));
}

void Shader::SetInt(const std::string& propertyName, int value)
{
	glUniform1i(glGetUniformLocation(programID, propertyName.c_str()), value);
}

void Shader::SetFloat(const std::string& propertyName, float value)
{
	glUniform1f(glGetUniformLocation(programID, propertyName.c_str()), value);
}

}
