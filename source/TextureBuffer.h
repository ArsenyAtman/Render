#pragma once

#include "Descriptor.h"

#include <vulkan/vulkan.h>

class Texture;

class TextureBuffer : public Descriptor
{
public:

	TextureBuffer(Render* render, Device* device, const ApplicationSettings* settings, const Texture* texture);
	virtual ~TextureBuffer();

	virtual VkDescriptorSetLayoutBinding getLayoutBinding() const override;
	virtual VkDescriptorPoolSize getPoolSize() const override;
	virtual VkWriteDescriptorSet getWriteSet(VkDescriptorSet descriptorSet) const override;

private:

	VkImage textureImage;
	VkDeviceMemory textureImageMemory;
	VkImageView textureImageView;

	VkSampler textureSampler;

	VkDescriptorImageInfo imageInfo;
};
