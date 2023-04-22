#include "Buffer.h"

#include "Helpers.h"
#include "Render.h"
#include "Device.h"
#include "Settings.h"

Buffer::Buffer(Render* render, Device* device, const ApplicationSettings* settings) : RenderModule(render, device, settings)
{
    // ...
}

Buffer::~Buffer()
{
    vkDestroyBuffer(getDevice()->getLogicalDevice(), buffer, nullptr);
    vkFreeMemory(getDevice()->getLogicalDevice(), bufferMemory, nullptr);
}

void Buffer::constructBuffer(const void* data, uint32_t elementSize, uint32_t countOfElements, VkBufferUsageFlagBits usageFlags)
{
    this->bufferSize = countOfElements;

    VkDeviceSize bufferMemorySize = elementSize * countOfElements;

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    Helpers::createBuffer(getDevice()->getLogicalDevice(), getDevice()->getPhysicalDevice(), bufferMemorySize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

    void* targetData;
    vkMapMemory(getDevice()->getLogicalDevice(), stagingBufferMemory, 0, bufferMemorySize, 0, &targetData);
    memcpy(targetData, data, (size_t)bufferMemorySize);
    vkUnmapMemory(getDevice()->getLogicalDevice(), stagingBufferMemory);

    Helpers::createBuffer(getDevice()->getLogicalDevice(), getDevice()->getPhysicalDevice(), bufferMemorySize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | usageFlags, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, buffer, bufferMemory);

    Helpers::copyBuffer(getDevice(), bufferMemorySize, stagingBuffer, buffer);

    vkDestroyBuffer(getDevice()->getLogicalDevice(), stagingBuffer, nullptr);
    vkFreeMemory(getDevice()->getLogicalDevice(), stagingBufferMemory, nullptr);
}