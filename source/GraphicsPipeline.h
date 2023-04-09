#pragma once

#include <vector>
#include <string>

#include <vulkan/vulkan.h>

class UniformBuffer;

class GraphicsPipeline
{

public:

	GraphicsPipeline(VkDevice logicalDevice, VkRenderPass renderPass, UniformBuffer* uniformBuffer);
	virtual ~GraphicsPipeline();

	void createDescriptorPool();
	void createDescriptorSets(UniformBuffer* uniformBuffer);

	VkDescriptorSetLayout descriptorSetLayout;
	VkDescriptorPool descriptorPool;
	std::vector<VkDescriptorSet> descriptorSets;

	VkPipelineLayout pipelineLayout;
	VkPipeline graphicsPipeline;

private:

	VkDevice logicalDevice;

	void createGraphicsPipeline(VkRenderPass renderPass);

	std::vector<char> readFile(const std::string& filename);

	VkShaderModule createShaderModule(const std::vector<char>& code);
};

