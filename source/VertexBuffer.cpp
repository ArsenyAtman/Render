#include "VertexBuffer.h"

#include <vector>
#include <stdexcept>

#include "Helpers.h"
#include "Mesh.h"

VertexBuffer::VertexBuffer(VkDevice logicalDevice, VkPhysicalDevice physicalDevice, VkQueue graphicsQueue, VkCommandPool commandPool, const Mesh* mesh)
{
	this->logicalDevice = logicalDevice;
	this->vertexBufferSize = static_cast<uint32_t>(mesh->getVertices().size());

    VkDeviceSize bufferSize = sizeof(mesh->getVertices()[0]) * mesh->getVertices().size();

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    Helpers::createBuffer(logicalDevice, physicalDevice, bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

    void* data;
    vkMapMemory(logicalDevice, stagingBufferMemory, 0, bufferSize, 0, &data);
    memcpy(data, mesh->getVertices().data(), (size_t)bufferSize);
    vkUnmapMemory(logicalDevice, stagingBufferMemory);

    Helpers::createBuffer(logicalDevice, physicalDevice, bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vertexBuffer, vertexBufferMemory);

	Helpers::copyBuffer(logicalDevice, bufferSize, graphicsQueue, commandPool, stagingBuffer, vertexBuffer);

    vkDestroyBuffer(logicalDevice, stagingBuffer, nullptr);
    vkFreeMemory(logicalDevice, stagingBufferMemory, nullptr);
}

VertexBuffer::~VertexBuffer()
{
	vkDestroyBuffer(logicalDevice, vertexBuffer, nullptr);
	vkFreeMemory(logicalDevice, vertexBufferMemory, nullptr);
}
