#pragma once

#include <array>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>
#include <vulkan/vulkan.h>

struct Vertex
{

public:

	glm::vec3 position;
	glm::vec3 color;
	glm::vec2 textureCoordinate;

	bool operator==(const Vertex& other) const;

	static VkVertexInputBindingDescription getBindingDescription();
	static std::array<VkVertexInputAttributeDescription, 3> getAttributeDescriptions();
};

namespace std
{
    template<> struct hash<Vertex>
    {
        size_t operator()(Vertex const& vertex) const
        {
            return ((hash<glm::vec3>()(vertex.position) ^ (hash<glm::vec3>()(vertex.color) << 1)) >> 1) ^ (hash<glm::vec2>()(vertex.textureCoordinate) << 1);
        }
    };
}
