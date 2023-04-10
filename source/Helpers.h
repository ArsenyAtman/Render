#pragma once

#include <vulkan/vulkan.h>

class Helpers
{

public:

    static void createBuffer(VkDevice logicalDevice, VkPhysicalDevice physicalDevice, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
    static void copyBuffer(VkDevice logicalDevice, VkDeviceSize size, VkQueue graphicsQueue, VkCommandPool commandPool, VkBuffer sourceBuffer, VkBuffer destinationBuffer);
    
    static uint32_t findMemoryType(VkPhysicalDevice physicalDevice, uint32_t typeFilter, VkMemoryPropertyFlags properties);

    static void createImage(VkDevice logicalDevice, VkPhysicalDevice physicalDevice, uint32_t textureWidth, uint32_t textureHeight, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& textureImage, VkDeviceMemory& textureImageMemory);
    static VkImageView createImageView(VkDevice logicalDevice, VkImage image, VkFormat format);

    static VkCommandBuffer beginSingleTimeCommands(VkDevice logicalDevice, VkCommandPool commandPool);
    static void endSingleTimeCommands(VkDevice logicalDevice, VkCommandBuffer commandBuffer, VkQueue graphicsQueue, VkCommandPool commandPool);
};
