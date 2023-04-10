#pragma once

#include <array>

#include <glm/glm.hpp>
#include <vulkan/vulkan.h>

struct Vertex
{

public:

	glm::vec2 pos;
	glm::vec3 color;
	glm::vec2 textureCoordinate;

	static VkVertexInputBindingDescription getBindingDescription();
	static std::array<VkVertexInputAttributeDescription, 3> getAttributeDescriptions();
};

