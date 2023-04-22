#pragma once

#include <vector>

#include <vulkan/vulkan.h>

class UniformBuffer;
class TextureImage;

struct ApplicationSettings;

class DescriptorsManager
{

public:
	DescriptorsManager(VkDevice logicalDevice, UniformBuffer* uniformBuffer, TextureImage* textureImage, const ApplicationSettings* settings);
	virtual ~DescriptorsManager();

	void createDescriptorPool(const ApplicationSettings* settings);
	void createDescriptorSets(UniformBuffer* uniformBuffer, TextureImage* textureImage, const ApplicationSettings* settings);

	VkDescriptorSetLayout descriptorSetLayout;
	VkDescriptorPool descriptorPool;
	std::vector<VkDescriptorSet> descriptorSets;

private:

	VkDevice logicalDevice;
};

