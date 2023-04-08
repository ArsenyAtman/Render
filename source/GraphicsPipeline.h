#pragma once

#include <vector>
#include <string>

#define GLFW_INCLUDE_VULKAN // Include Vulkan inside GLFW
#include <GLFW/glfw3.h>

class GraphicsPipeline
{

public:

	GraphicsPipeline(VkDevice logicalDevice, VkRenderPass renderPass);
	virtual ~GraphicsPipeline();

	VkPipelineLayout pipelineLayout;
	VkPipeline graphicsPipeline;

private:

	VkDevice logicalDevice;

	void createGraphicsPipeline(VkRenderPass renderPass);

	std::vector<char> readFile(const std::string& filename);

	VkShaderModule createShaderModule(const std::vector<char>& code);
};

