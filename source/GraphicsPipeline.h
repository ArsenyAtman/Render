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

	VkPipelineLayout pipelineLayout;
	VkPipeline graphicsPipeline;

private:

	VkDevice logicalDevice;

	void createGraphicsPipeline(VkRenderPass renderPass, UniformBuffer* uniformBuffer);

	std::vector<char> readFile(const std::string& filename);

	VkShaderModule createShaderModule(const std::vector<char>& code);
};

