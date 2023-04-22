#pragma once

#include "RenderModule.h"
#include "CommandBufferBindable.h"

#include <vector>
#include <string>

#include <vulkan/vulkan.h>

class Shader;

class GraphicsPipeline : public RenderModule, public CommandBufferBindable
{

public:

	GraphicsPipeline(Render* render, Device* device, const ApplicationSettings* settings, const std::vector<Shader*>& shaders);
	virtual ~GraphicsPipeline();

	virtual void bindToCommandBuffer(VkCommandBuffer commandBuffer) const override;

private:

	void createGraphicsPipeline(const std::vector<Shader*>& shaders);
	VkShaderModule createShaderModule(const std::vector<char>& code) const;
	VkShaderStageFlagBits getShaderFlags(const Shader* shader) const;
	VkPipelineShaderStageCreateInfo createShaderStageInfo(VkShaderModule shaderModule, VkShaderStageFlagBits stageFlags) const;

	VkPipelineLayout pipelineLayout;
	VkPipeline graphicsPipeline;

};
