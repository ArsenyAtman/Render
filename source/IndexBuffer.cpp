#include "IndexBuffer.h"

#include "Helpers.h"
#include "Mesh.h"

IndexBuffer::IndexBuffer(VkDevice logicalDevice, VkPhysicalDevice physicalDevice, VkQueue graphicsQueue, VkCommandPool commandPool, const Mesh* mesh)
{
	this->logicalDevice = logicalDevice;
    this->indexBufferSize = static_cast<uint32_t>(mesh->getIndices().size());

    VkDeviceSize bufferSize = sizeof(mesh->getIndices()[0]) * mesh->getIndices().size();

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    Helpers::createBuffer(logicalDevice, physicalDevice, bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

    void* data;
    vkMapMemory(logicalDevice, stagingBufferMemory, 0, bufferSize, 0, &data);
    memcpy(data, mesh->getIndices().data(), (size_t)bufferSize);
    vkUnmapMemory(logicalDevice, stagingBufferMemory);

    Helpers::createBuffer(logicalDevice, physicalDevice, bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, indexBuffer, indexBufferMemory);

    Helpers::copyBuffer(logicalDevice, bufferSize, graphicsQueue, commandPool, stagingBuffer, indexBuffer);

    vkDestroyBuffer(logicalDevice, stagingBuffer, nullptr);
    vkFreeMemory(logicalDevice, stagingBufferMemory, nullptr);
}

IndexBuffer::~IndexBuffer()
{
	vkDestroyBuffer(logicalDevice, indexBuffer, nullptr);
	vkFreeMemory(logicalDevice, indexBufferMemory, nullptr);
}