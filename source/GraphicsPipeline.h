#pragma once

#include "RenderModule.h"
#include "CommandBufferBindable.h"
#include "Tickable.h"

#include <vector>
#include <string>

#include <vulkan/vulkan.h>

class Model;
class Shader;
class DescriptorsManager;

class GraphicsPipeline : public RenderModule, public CommandBufferBindable, public Tickable
{
public:

	GraphicsPipeline(Render* render, Device* device, const ApplicationSettings* settings, const Model* model);
	virtual ~GraphicsPipeline();

	virtual void bindToCommandBuffer(VkCommandBuffer commandBuffer) const override;

	virtual void tick() override;

private:

	DescriptorsManager* descriptorsManager;

	void createGraphicsPipeline(const std::vector<Shader*>& shaders);
	VkShaderModule createShaderModule(const std::vector<char>& code) const;
	VkShaderStageFlagBits getShaderFlags(const Shader* shader) const;
	VkPipelineShaderStageCreateInfo createShaderStageInfo(VkShaderModule shaderModule, VkShaderStageFlagBits stageFlags) const;

	VkPipelineLayout pipelineLayout;
	VkPipeline graphicsPipeline;

};
