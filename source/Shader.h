#pragma once

#include <string>
#include <vector>

enum class ShaderType : uint8_t
{
	Vertex,
	Fragment
};

class Shader
{
public:

	Shader(ShaderType shaderType, const std::vector<char>& shaderCode);

	ShaderType getType() const { return shaderType; }
	std::vector<char> getCode() const { return code; }

private:

	ShaderType shaderType;
	std::vector<char> code;
};

