#pragma once

#include <vector>

#include <vulkan/vulkan.h>

class UniformBuffer;
class TextureImage;

class DescriptorsManager
{

public:
	DescriptorsManager(VkDevice logicalDevice, UniformBuffer* uniformBuffer, TextureImage* textureImage);
	virtual ~DescriptorsManager();

	void createDescriptorPool();
	void createDescriptorSets(UniformBuffer* uniformBuffer, TextureImage* textureImage);

	VkDescriptorSetLayout descriptorSetLayout;
	VkDescriptorPool descriptorPool;
	std::vector<VkDescriptorSet> descriptorSets;

private:

	VkDevice logicalDevice;
};

