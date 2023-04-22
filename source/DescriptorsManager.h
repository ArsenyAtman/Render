#pragma once

#include "RenderModule.h"

#include <vector>

#include <vulkan/vulkan.h>

class DescriptorsManager : public RenderModule
{
public:

	DescriptorsManager(Render* render, Device* device, const ApplicationSettings* settings);
	virtual ~DescriptorsManager();

	const VkDescriptorSetLayout* getDescriptorSetLayout() const;
	const VkDescriptorSet* getDescriptorSetForCurrentFrame() const;

private:

	void createDescriptorSetLayout();
	void createDescriptorPool();
	void createDescriptorSets();

	VkDescriptorSetLayout descriptorSetLayout;
	VkDescriptorPool descriptorPool;
	std::vector<VkDescriptorSet> descriptorSets;
};
