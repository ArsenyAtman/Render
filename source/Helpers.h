#pragma once

#include <vector>

#include <vulkan/vulkan.h>

class Device;

class Helpers
{
public:

    static void createBuffer(Device* device, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
    static void copyBuffer(Device* device, VkDeviceSize size, VkBuffer sourceBuffer, VkBuffer destinationBuffer);
    
    static uint32_t findMemoryType(Device* device, uint32_t typeFilter, VkMemoryPropertyFlags properties);

    static void createImage(Device* device, uint32_t textureWidth, uint32_t textureHeight, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& textureImage, VkDeviceMemory& textureImageMemory);
    static VkImageView createImageView(Device* device, VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);

    static void copyBufferToImage(Device* device, VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);

    static void beginSingleTimeCommands(Device* device, VkCommandBuffer& outCommandBuffer, VkCommandPool& outCommandPool);
    static void endSingleTimeCommands(Device* device, VkCommandBuffer commandBuffer, VkCommandPool commandPool);

    static void transitionImageLayout(Device* device, VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);

    static VkFormat findSupportedFormat(Device* device, const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
    static VkFormat findDepthFormat(Device* device);
    static bool hasStencilComponent(VkFormat format);
};
