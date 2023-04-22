#include "DescriptorsManager.h"

#include <stdexcept>
#include <array>

#include "UniformBuffer.h"
#include "TextureImage.h"
#include "Settings.h"

DescriptorsManager::DescriptorsManager(VkDevice logicalDevice, UniformBuffer* uniformBuffer, TextureImage* textureImage, const ApplicationSettings* settings)
{
	this->logicalDevice = logicalDevice;

	std::array<VkDescriptorSetLayoutBinding, 2> bindings = { uniformBuffer->uboLayoutBinding, textureImage->samplerLayoutBinding };
	VkDescriptorSetLayoutCreateInfo layoutInfo{};
	layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
	layoutInfo.pBindings = bindings.data();

	VkResult result = vkCreateDescriptorSetLayout(logicalDevice, &layoutInfo, nullptr, &descriptorSetLayout);
	if (result != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create descriptor set layout!");
	}

	createDescriptorPool(settings);
	createDescriptorSets(uniformBuffer, textureImage, settings);
}

DescriptorsManager::~DescriptorsManager()
{
	vkDestroyDescriptorPool(logicalDevice, descriptorPool, nullptr);
	vkDestroyDescriptorSetLayout(logicalDevice, descriptorSetLayout, nullptr);
}

void DescriptorsManager::createDescriptorPool(const ApplicationSettings* settings)
{
	std::array<VkDescriptorPoolSize, 2> poolSizes{};
	poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	poolSizes[0].descriptorCount = static_cast<uint32_t>(settings->maxFramesInFlight);
	poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	poolSizes[1].descriptorCount = static_cast<uint32_t>(settings->maxFramesInFlight);

	VkDescriptorPoolCreateInfo poolInfo{};
	poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
	poolInfo.pPoolSizes = poolSizes.data();
	poolInfo.maxSets = static_cast<uint32_t>(settings->maxFramesInFlight);

	VkResult result = vkCreateDescriptorPool(logicalDevice, &poolInfo, nullptr, &descriptorPool);
	if (result != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create descriptor pool!");
	}
}

void DescriptorsManager::createDescriptorSets(UniformBuffer* uniformBuffer, TextureImage* textureImage, const ApplicationSettings* settings)
{
	std::vector<VkDescriptorSetLayout> layouts(settings->maxFramesInFlight, descriptorSetLayout);
	VkDescriptorSetAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = descriptorPool;
	allocInfo.descriptorSetCount = static_cast<uint32_t>(settings->maxFramesInFlight);
	allocInfo.pSetLayouts = layouts.data();

	descriptorSets.resize(settings->maxFramesInFlight);

	VkResult result = vkAllocateDescriptorSets(logicalDevice, &allocInfo, descriptorSets.data());
	if (result != VK_SUCCESS)
	{
		throw std::runtime_error("failed to allocate descriptor sets!");
	}

	for (size_t i = 0; i < settings->maxFramesInFlight; i++)
	{
		VkDescriptorBufferInfo bufferInfo{};
		bufferInfo.buffer = uniformBuffer->uniformBuffers[i];
		bufferInfo.offset = 0;
		bufferInfo.range = sizeof(UniformBufferObject);

		VkDescriptorImageInfo imageInfo{};
		imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		imageInfo.imageView = textureImage->textureImageView;
		imageInfo.sampler = textureImage->textureSampler;

		std::array<VkWriteDescriptorSet, 2> descriptorWrites{};

		descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrites[0].dstSet = descriptorSets[i];
		descriptorWrites[0].dstBinding = 0;
		descriptorWrites[0].dstArrayElement = 0;
		descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		descriptorWrites[0].descriptorCount = 1;
		descriptorWrites[0].pBufferInfo = &bufferInfo;

		descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrites[1].dstSet = descriptorSets[i];
		descriptorWrites[1].dstBinding = 1;
		descriptorWrites[1].dstArrayElement = 0;
		descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		descriptorWrites[1].descriptorCount = 1;
		descriptorWrites[1].pImageInfo = &imageInfo;

		vkUpdateDescriptorSets(logicalDevice, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
	}
}