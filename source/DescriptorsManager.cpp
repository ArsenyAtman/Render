#include "DescriptorsManager.h"

#include <stdexcept>
#include <array>

#include "Render.h"
#include "Device.h"
#include "Settings.h"
#include "UniformBuffer.h"
#include "TextureImage.h"

DescriptorsManager::DescriptorsManager(Render* render, Device* device, const ApplicationSettings* settings) : RenderModule(render, device, settings)
{
	createDescriptorSetLayout();
	createDescriptorPool();
	createDescriptorSets();
}

DescriptorsManager::~DescriptorsManager()
{
	vkDestroyDescriptorPool(getDevice()->getLogicalDevice(), descriptorPool, nullptr);
	vkDestroyDescriptorSetLayout(getDevice()->getLogicalDevice(), descriptorSetLayout, nullptr);
}

const VkDescriptorSetLayout* DescriptorsManager::getDescriptorSetLayout() const
{
	return &descriptorSetLayout;
}

const VkDescriptorSet* DescriptorsManager::getDescriptorSetForCurrentFrame() const
{
	return &descriptorSets[getRender()->getCurrentFrame()];
}

void DescriptorsManager::createDescriptorSetLayout()
{
	std::array<VkDescriptorSetLayoutBinding, 2> bindings = { getRender()->getUniformBuffer()->uboLayoutBinding, getRender()->getTextureImage()->samplerLayoutBinding};
	VkDescriptorSetLayoutCreateInfo layoutInfo{};
	layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
	layoutInfo.pBindings = bindings.data();

	VkResult result = vkCreateDescriptorSetLayout(getDevice()->getLogicalDevice(), &layoutInfo, nullptr, &descriptorSetLayout);
	if (result != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to create descriptor set layout!");
	}
}

void DescriptorsManager::createDescriptorPool()
{
	std::array<VkDescriptorPoolSize, 2> poolSizes{};
	poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	poolSizes[0].descriptorCount = static_cast<uint32_t>(getSettings()->maxFramesInFlight);
	poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	poolSizes[1].descriptorCount = static_cast<uint32_t>(getSettings()->maxFramesInFlight);

	VkDescriptorPoolCreateInfo poolInfo{};
	poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
	poolInfo.pPoolSizes = poolSizes.data();
	poolInfo.maxSets = static_cast<uint32_t>(getSettings()->maxFramesInFlight);

	VkResult result = vkCreateDescriptorPool(getDevice()->getLogicalDevice(), &poolInfo, nullptr, &descriptorPool);
	if (result != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create descriptor pool!");
	}
}

void DescriptorsManager::createDescriptorSets()
{
	std::vector<VkDescriptorSetLayout> layouts(getSettings()->maxFramesInFlight, descriptorSetLayout);
	VkDescriptorSetAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = descriptorPool;
	allocInfo.descriptorSetCount = static_cast<uint32_t>(getSettings()->maxFramesInFlight);
	allocInfo.pSetLayouts = layouts.data();

	descriptorSets.resize(getSettings()->maxFramesInFlight);

	VkResult result = vkAllocateDescriptorSets(getDevice()->getLogicalDevice(), &allocInfo, descriptorSets.data());
	if (result != VK_SUCCESS)
	{
		throw std::runtime_error("failed to allocate descriptor sets!");
	}

	for (size_t i = 0; i < getSettings()->maxFramesInFlight; i++)
	{
		VkDescriptorBufferInfo bufferInfo{};
		bufferInfo.buffer = getRender()->getUniformBuffer()->uniformBuffers[i];
		bufferInfo.offset = 0;
		bufferInfo.range = sizeof(UniformBufferObject);

		VkDescriptorImageInfo imageInfo{};
		imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		imageInfo.imageView = getRender()->getTextureImage()->textureImageView;
		imageInfo.sampler = getRender()->getTextureImage()->textureSampler;

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

		vkUpdateDescriptorSets(getDevice()->getLogicalDevice(), static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
	}
}