#pragma once

#include <string>

namespace Graphics
{

class Shader {
	
public:
	Shader(const std::string& vertexShaderPath, const std::string& fragShaderPath);
	
	void Use();
	
	void SetBool(const std::string& propertyName, bool value);
	void SetInt(const std::string& propertyName, int value);
	void SetFloat(const std::string& propertyName, float value);

private:
	unsigned int programID = 0;
};

}