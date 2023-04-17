#include "VertexBuffer.h"

#include <vector>
#include <stdexcept>

#include "Helpers.h"
#include "Mesh.h"

VertexBuffer::VertexBuffer(VkDevice logicalDevice, VkPhysicalDevice physicalDevice, const Mesh* mesh)
{
	this->logicalDevice = logicalDevice;
	this->vertexBufferSize = static_cast<uint32_t>(mesh->getVertices().size());

	VkBufferCreateInfo bufferInfo{};
	bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferInfo.size = sizeof(mesh->getVertices()[0]) * mesh->getVertices().size();
	bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
	bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	VkResult result = vkCreateBuffer(logicalDevice, &bufferInfo, nullptr, &vertexBuffer);
	if (result != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create vertex buffer!");
	}

	VkMemoryRequirements memRequirements;
	vkGetBufferMemoryRequirements(logicalDevice, vertexBuffer, &memRequirements);

	VkMemoryAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = memRequirements.size;
	allocInfo.memoryTypeIndex = Helpers::findMemoryType(physicalDevice, memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

	VkResult result2 = vkAllocateMemory(logicalDevice, &allocInfo, nullptr, &vertexBufferMemory);
	if (result2 != VK_SUCCESS)
	{
		throw std::runtime_error("failed to allocate vertex buffer memory!");
	}

	vkBindBufferMemory(logicalDevice, vertexBuffer, vertexBufferMemory, 0);

	void* data;
	vkMapMemory(logicalDevice, vertexBufferMemory, 0, bufferInfo.size, 0, &data);
	memcpy(data, mesh->getVertices().data(), static_cast<size_t>(bufferInfo.size));
	vkUnmapMemory(logicalDevice, vertexBufferMemory);
}

VertexBuffer::~VertexBuffer()
{
	vkDestroyBuffer(logicalDevice, vertexBuffer, nullptr);
	vkFreeMemory(logicalDevice, vertexBufferMemory, nullptr);
}
