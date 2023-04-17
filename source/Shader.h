#pragma once

#include <vector>

#include <vulkan/vulkan.h>

class Shader
{
public:

	Shader(std::vector<char>& shaderCode);

	std::vector<char> getCode() const { return code; }

private:

	std::vector<char> code;
};

