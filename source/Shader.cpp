#include "Shader.h"

Shader::Shader(ShaderType shaderType, const std::vector<char>& shaderCode)
{
	this->shaderType = shaderType;
	this->code = shaderCode;
}
