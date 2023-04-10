#pragma once

#include <vector>
#include <string>

#include <vulkan/vulkan.h>

class DescriptorsManager;

class GraphicsPipeline
{

public:

	GraphicsPipeline(VkDevice logicalDevice, VkRenderPass renderPass, DescriptorsManager* descriptorsManager);
	virtual ~GraphicsPipeline();

	VkPipelineLayout pipelineLayout;
	VkPipeline graphicsPipeline;

private:

	VkDevice logicalDevice;

	void createGraphicsPipeline(VkRenderPass renderPass, DescriptorsManager* descriptorsManager);

	std::vector<char> readFile(const std::string& filename);

	VkShaderModule createShaderModule(const std::vector<char>& code);
};

