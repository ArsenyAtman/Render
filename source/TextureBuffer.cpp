#include "TextureBuffer.h"

#include <stdexcept>

#include "Render.h"
#include "Device.h"
#include "Settings.h"
#include "Texture.h"
#include "Helpers.h"

TextureBuffer::TextureBuffer(Render* render, Device* device, const ApplicationSettings* settings, const Texture* texture) : Descriptor(render, device, settings)
{
	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;

	Helpers::createBuffer(getDevice()->getLogicalDevice(), getDevice()->getPhysicalDevice(), texture->getSize(), VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

	void* data;
	vkMapMemory(getDevice()->getLogicalDevice(), stagingBufferMemory, 0, texture->getSize(), 0, &data);
	memcpy(data, texture->getData(), texture->getSize());
	vkUnmapMemory(getDevice()->getLogicalDevice(), stagingBufferMemory);

	Helpers::createImage(getDevice()->getLogicalDevice(), getDevice()->getPhysicalDevice(), texture->getWidth(), texture->getHeight(), VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, textureImage, textureImageMemory);

	Helpers::transitionImageLayout(getDevice(), textureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
	Helpers::copyBufferToImage(getDevice(), stagingBuffer, textureImage, texture->getWidth(), texture->getHeight());

	Helpers::transitionImageLayout(getDevice(), textureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

	vkDestroyBuffer(getDevice()->getLogicalDevice(), stagingBuffer, nullptr);
	vkFreeMemory(getDevice()->getLogicalDevice(), stagingBufferMemory, nullptr);

	textureImageView = Helpers::createImageView(getDevice()->getLogicalDevice(), textureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT);

	VkPhysicalDeviceProperties properties{};
	vkGetPhysicalDeviceProperties(getDevice()->getPhysicalDevice(), &properties);

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

	VkResult result = vkCreateSampler(getDevice()->getLogicalDevice(), &samplerInfo, nullptr, &textureSampler);
	if (result != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create texture sampler!");
	}

	imageInfo = VkDescriptorImageInfo();
	imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	imageInfo.imageView = textureImageView;
	imageInfo.sampler = textureSampler;
}

TextureBuffer::~TextureBuffer()
{
	vkDestroySampler(getDevice()->getLogicalDevice(), textureSampler, nullptr);

	vkDestroyImageView(getDevice()->getLogicalDevice(), textureImageView, nullptr);
	vkDestroyImage(getDevice()->getLogicalDevice(), textureImage, nullptr);
	vkFreeMemory(getDevice()->getLogicalDevice(), textureImageMemory, nullptr);
}

VkDescriptorSetLayoutBinding TextureBuffer::getLayoutBinding() const
{
	VkDescriptorSetLayoutBinding layoutBinding = VkDescriptorSetLayoutBinding();
	layoutBinding.binding = 1;
	layoutBinding.descriptorCount = 1;
	layoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	layoutBinding.pImmutableSamplers = nullptr;
	layoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

	return layoutBinding;
}

VkDescriptorPoolSize TextureBuffer::getPoolSize() const
{
	VkDescriptorPoolSize poolSize = VkDescriptorPoolSize();
	poolSize.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	poolSize.descriptorCount = static_cast<uint32_t>(getSettings()->maxFramesInFlight);

	return poolSize;
}

VkWriteDescriptorSet TextureBuffer::getWriteSet(VkDescriptorSet descriptorSet) const
{
	VkWriteDescriptorSet writeSet = VkWriteDescriptorSet();
	writeSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	writeSet.dstSet = descriptorSet;
	writeSet.dstBinding = 1;
	writeSet.dstArrayElement = 0;
	writeSet.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	writeSet.descriptorCount = 1;
	writeSet.pImageInfo = &imageInfo;

	return writeSet;
}
