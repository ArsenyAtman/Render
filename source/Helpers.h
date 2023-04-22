#pragma once

#include <vector>

#include <vulkan/vulkan.h>

class Device;

class Helpers
{

public:

    static void createBuffer(VkDevice logicalDevice, VkPhysicalDevice physicalDevice, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
    static void copyBuffer(Device* device, VkDeviceSize size, VkBuffer sourceBuffer, VkBuffer destinationBuffer);
    
    static uint32_t findMemoryType(VkPhysicalDevice physicalDevice, uint32_t typeFilter, VkMemoryPropertyFlags properties);

    static void createImage(VkDevice logicalDevice, VkPhysicalDevice physicalDevice, uint32_t textureWidth, uint32_t textureHeight, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& textureImage, VkDeviceMemory& textureImageMemory);
    static VkImageView createImageView(VkDevice logicalDevice, VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);

    static VkCommandBuffer beginSingleTimeCommands(VkDevice logicalDevice, VkCommandPool commandPool);
    static void endSingleTimeCommands(VkDevice logicalDevice, VkCommandBuffer commandBuffer, VkQueue graphicsQueue, VkCommandPool commandPool);

    static void transitionImageLayout(VkDevice logicalDevice, VkQueue graphicsQueue, VkCommandPool commandPool, VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);

    static VkFormat findSupportedFormat(VkPhysicalDevice physicalDevice, const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
    static VkFormat findDepthFormat(VkPhysicalDevice physicalDevice);
    static bool hasStencilComponent(VkFormat format);
};
