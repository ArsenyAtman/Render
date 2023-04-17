#pragma once

#include <vector>
#include <string>

#include <vulkan/vulkan.h>

class Shader;
class DescriptorsManager;

class GraphicsPipeline
{

public:

	GraphicsPipeline(VkDevice logicalDevice, VkRenderPass renderPass, DescriptorsManager* descriptorsManager, const std::vector<Shader*>& shaders);
	virtual ~GraphicsPipeline();

	VkPipelineLayout pipelineLayout;
	VkPipeline graphicsPipeline;

private:

	VkDevice logicalDevice;

	void createGraphicsPipeline(VkRenderPass renderPass, DescriptorsManager* descriptorsManager, const std::vector<Shader*>& shaders);

	VkShaderModule createShaderModule(const std::vector<char>& code);
};

