#include "TextureImage.h"

#include <stdexcept>

#include "Texture.h"
#include "Helpers.h"

TextureImage::TextureImage(VkDevice logicalDevice, VkPhysicalDevice physicalDevice, VkQueue graphicsQueue, VkCommandPool commandPool, const Texture* texture)
{
	this->logicalDevice = logicalDevice;

	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;

	Helpers::createBuffer(logicalDevice, physicalDevice, texture->getSize(), VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

	void* data;
	vkMapMemory(logicalDevice, stagingBufferMemory, 0, texture->getSize(), 0, &data);
	memcpy(data, texture->getData(), texture->getSize());
	vkUnmapMemory(logicalDevice, stagingBufferMemory);

	Helpers::createImage(logicalDevice, physicalDevice, texture->getWidth(), texture->getHeight(), VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, textureImage, textureImageMemory);

	Helpers::transitionImageLayout(logicalDevice, graphicsQueue, commandPool, textureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
	copyBufferToImage(logicalDevice, graphicsQueue, commandPool, stagingBuffer, textureImage, texture->getWidth(), texture->getHeight());

	Helpers::transitionImageLayout(logicalDevice, graphicsQueue, commandPool, textureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

	vkDestroyBuffer(logicalDevice, stagingBuffer, nullptr);
	vkFreeMemory(logicalDevice, stagingBufferMemory, nullptr);

	textureImageView = Helpers::createImageView(logicalDevice, textureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT);

	VkPhysicalDeviceProperties properties{};
	vkGetPhysicalDeviceProperties(physicalDevice, &properties);

	VkSamplerCreateInfo samplerInfo{};
	samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
	samplerInfo.magFilter = VK_FILTER_LINEAR;
	samplerInfo.minFilter = VK_FILTER_LINEAR;
	samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	samplerInfo.anisotropyEnable = VK_TRUE;
	samplerInfo.maxAnisotropy = properties.limits.maxSamplerAnisotropy;
	samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
	samplerInfo.unnormalizedCoordinates = VK_FALSE;
	samplerInfo.compareEnable = VK_FALSE;
	samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
	samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
	samplerInfo.mipLodBias = 0.0f;
	samplerInfo.minLod = 0.0f;
	samplerInfo.maxLod = 0.0f;

	VkResult result = vkCreateSampler(logicalDevice, &samplerInfo, nullptr, &textureSampler);
	if (result != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create texture sampler!");
	}

	samplerLayoutBinding = VkDescriptorSetLayoutBinding();
	samplerLayoutBinding.binding = 1;
	samplerLayoutBinding.descriptorCount = 1;
	samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	samplerLayoutBinding.pImmutableSamplers = nullptr;
	samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
}

TextureImage::~TextureImage()
{
	vkDestroySampler(logicalDevice, textureSampler, nullptr);

	vkDestroyImageView(logicalDevice, textureImageView, nullptr);

	vkDestroyImage(logicalDevice, textureImage, nullptr);
	vkFreeMemory(logicalDevice, textureImageMemory, nullptr);
}

void TextureImage::copyBufferToImage(VkDevice logicalDevice, VkQueue graphicsQueue, VkCommandPool commandPool, VkBuffer buffer, VkImage image, uint32_t width, uint32_t height)
{
	VkCommandBuffer commandBuffer = Helpers::beginSingleTimeCommands(logicalDevice, commandPool);

	VkBufferImageCopy region{};
	region.bufferOffset = 0;
	region.bufferRowLength = 0;
	region.bufferImageHeight = 0;

	region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	region.imageSubresource.mipLevel = 0;
	region.imageSubresource.baseArrayLayer = 0;
	region.imageSubresource.layerCount = 1;

	region.imageOffset = { 0, 0, 0 };
	region.imageExtent = { width, height, 1 };

	vkCmdCopyBufferToImage(commandBuffer, buffer, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);

	Helpers::endSingleTimeCommands(logicalDevice, commandBuffer, graphicsQueue, commandPool);
}