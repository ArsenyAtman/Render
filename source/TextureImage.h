#pragma once

#include <vulkan/vulkan.h>

class Texture;

class TextureImage
{

public:

	TextureImage(VkDevice logicalDevice, VkPhysicalDevice physicalDevice, VkQueue graphicsQueue, VkCommandPool commandPool, const Texture* texture);
	virtual ~TextureImage();

	void copyBufferToImage(VkDevice logicalDevice, VkQueue graphicsQueue, VkCommandPool commandPool, VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);

	VkImage textureImage;
	VkDeviceMemory textureImageMemory;

	VkImageView textureImageView;

	VkSampler textureSampler;

	VkDescriptorSetLayoutBinding samplerLayoutBinding;

private:

	VkDevice logicalDevice;
};

