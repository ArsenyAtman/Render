#include "GraphicsPipeline.h"

#include <stdexcept>
#include <fstream>

#include "Vertex.h"
#include "Shader.h"
#include "DescriptorsManager.h"
#include "SwapChain.h"
#include "Render.h"
#include "Device.h"
#include "Settings.h"
#include "Model.h"

GraphicsPipeline::GraphicsPipeline(Render* render, Device* device, const ApplicationSettings* settings, const Model* model) : RenderModule(render, device, settings)
{
	descriptorsManager = new DescriptorsManager(render, device, settings, model);

	createGraphicsPipeline(model->getShaders());
}

GraphicsPipeline::~GraphicsPipeline()
{
	vkDestroyPipeline(getDevice()->getLogicalDevice(), graphicsPipeline, nullptr);
	vkDestroyPipelineLayout(getDevice()->getLogicalDevice(), pipelineLayout, nullptr);

	delete descriptorsManager;
}

void GraphicsPipeline::bindToCommandBuffer(VkCommandBuffer commandBuffer) const
{
	vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);
	vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1, &descriptorsManager->getDescriptorSetForCurrentFrame(), 0, nullptr);
}

void GraphicsPipeline::tick()
{
	descriptorsManager->tick();
}

void GraphicsPipeline::createGraphicsPipeline(const std::vector<Shader*>& shaders)
{
	std::vector<VkShaderModule> shaderModules;
	std::vector<VkPipelineShaderStageCreateInfo> shaderStages;

	for (size_t i = 0; i < shaders.size(); ++i)
	{
		shaderModules.push_back(createShaderModule(shaders[i]->getCode()));
		VkShaderStageFlagBits shaderFlags = getShaderFlags(shaders[i]);
		shaderStages.push_back(createShaderStageInfo(shaderModules[i], shaderFlags));
	}

	VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
	vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	vertexInputInfo.vertexBindingDescriptionCount = 0;
	vertexInputInfo.vertexAttributeDescriptionCount = 0;
	VkVertexInputBindingDescription bindingDescription = Vertex::getBindingDescription();
	std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions = Vertex::getAttributeDescriptions();
	vertexInputInfo.vertexBindingDescriptionCount = 1;
	vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
	vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
	vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

	VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
	inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	inputAssembly.primitiveRestartEnable = VK_FALSE;

	VkPipelineViewportStateCreateInfo viewportState{};
	viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewportState.viewportCount = 1;
	viewportState.scissorCount = 1;

	VkPipelineRasterizationStateCreateInfo rasterizer{};
	rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	rasterizer.depthClampEnable = VK_FALSE;
	rasterizer.rasterizerDiscardEnable = VK_FALSE;
	rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
	rasterizer.lineWidth = 1.0f;
	rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
	rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
	rasterizer.depthBiasEnable = VK_FALSE;

	VkPipelineMultisampleStateCreateInfo multisampling{};
	multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	multisampling.sampleShadingEnable = VK_FALSE;
	multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

	VkPipelineColorBlendAttachmentState colorBlendAttachment{};
	colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	colorBlendAttachment.blendEnable = VK_FALSE;

	VkPipelineColorBlendStateCreateInfo colorBlending{};
	colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	colorBlending.logicOpEnable = VK_FALSE;
	colorBlending.logicOp = VK_LOGIC_OP_COPY;
	colorBlending.attachmentCount = 1;
	colorBlending.pAttachments = &colorBlendAttachment;
	colorBlending.blendConstants[0] = 0.0f;
	colorBlending.blendConstants[1] = 0.0f;
	colorBlending.blendConstants[2] = 0.0f;
	colorBlending.blendConstants[3] = 0.0f;

	std::vector<VkDynamicState> dynamicStates = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR };

	VkPipelineDynamicStateCreateInfo dynamicState{};
	dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
	dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
	dynamicState.pDynamicStates = dynamicStates.data();

	VkPipelineDepthStencilStateCreateInfo depthStencil{};
	depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
	depthStencil.depthTestEnable = VK_TRUE;
	depthStencil.depthWriteEnable = VK_TRUE;
	depthStencil.depthCompareOp = VK_COMPARE_OP_LESS;
	depthStencil.depthBoundsTestEnable = VK_FALSE;
	depthStencil.stencilTestEnable = VK_FALSE;

	VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.setLayoutCount = 1;
	pipelineLayoutInfo.pSetLayouts = &descriptorsManager->getDescriptorSetLayout();
	pipelineLayoutInfo.pushConstantRangeCount = 0;

	VkResult layoutCreationResult = vkCreatePipelineLayout(getDevice()->getLogicalDevice(), &pipelineLayoutInfo, nullptr, &pipelineLayout);
	if (layoutCreationResult != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to create a pipeline layout!");
	}

	VkGraphicsPipelineCreateInfo pipelineInfo{};
	pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	pipelineInfo.stageCount = static_cast<uint32_t>(shaderStages.size());
	pipelineInfo.pStages = shaderStages.data();
	pipelineInfo.pVertexInputState = &vertexInputInfo;
	pipelineInfo.pInputAssemblyState = &inputAssembly;
	pipelineInfo.pViewportState = &viewportState;
	pipelineInfo.pRasterizationState = &rasterizer;
	pipelineInfo.pMultisampleState = &multisampling;
	pipelineInfo.pColorBlendState = &colorBlending;
	pipelineInfo.pDynamicState = &dynamicState;
	pipelineInfo.layout = pipelineLayout;
	pipelineInfo.renderPass = getRender()->getSwapChain()->getRenderPass();
	pipelineInfo.subpass = 0;
	pipelineInfo.pDepthStencilState = &depthStencil;

	VkResult pipelineCreationResult = vkCreateGraphicsPipelines(getDevice()->getLogicalDevice(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &graphicsPipeline);
	if (pipelineCreationResult != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create graphics pipeline!");
	}

	for (VkShaderModule& shaderModule : shaderModules)
	{
		vkDestroyShaderModule(getDevice()->getLogicalDevice(), shaderModule, nullptr);
	}
}

VkShaderModule GraphicsPipeline::createShaderModule(const std::vector<char>& shaderCode) const
{
	VkShaderModuleCreateInfo createInfo = VkShaderModuleCreateInfo();
	createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	createInfo.codeSize = shaderCode.size();
	createInfo.pCode = reinterpret_cast<const uint32_t*>(shaderCode.data());

	VkShaderModule shaderModule;
	VkResult result = vkCreateShaderModule(getDevice()->getLogicalDevice(), &createInfo, nullptr, &shaderModule);
	if (result != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to create a shader module!");
	}

	return shaderModule;
}

VkShaderStageFlagBits GraphicsPipeline::getShaderFlags(const Shader* shader) const
{
	switch (shader->getType())
	{
	case ShaderType::Fragment:
		return VK_SHADER_STAGE_FRAGMENT_BIT;
	case ShaderType::Vertex:
		return VK_SHADER_STAGE_VERTEX_BIT;
	default:
		return VK_SHADER_STAGE_FLAG_BITS_MAX_ENUM;
	}
}

VkPipelineShaderStageCreateInfo GraphicsPipeline::createShaderStageInfo(VkShaderModule shaderModule, VkShaderStageFlagBits stageFlags) const
{
	VkPipelineShaderStageCreateInfo shaderStageInfo{};
	shaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	shaderStageInfo.stage = stageFlags;
	shaderStageInfo.module = shaderModule;
	shaderStageInfo.pName = "main";

	return shaderStageInfo;
}
