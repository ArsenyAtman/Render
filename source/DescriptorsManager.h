#pragma once

#include "RenderModule.h"
#include "Tickable.h"

#include <vector>

#include <vulkan/vulkan.h>

class Model;
class Descriptor;

class DescriptorsManager : public RenderModule, public Tickable
{
public:

	DescriptorsManager(Render* render, Device* device, const ApplicationSettings* settings, const Model* model);
	virtual ~DescriptorsManager();

	virtual void tick() override;

	const VkDescriptorSetLayout& getDescriptorSetLayout() const;
	const VkDescriptorSet& getDescriptorSetForCurrentFrame() const;

private:

	void createDescriptorSetLayout();
	void createDescriptorPool();
	void createDescriptorSets();

	std::vector<std::vector<Descriptor*>> descriptorsForFrames;

	VkDescriptorSetLayout descriptorSetLayout;
	VkDescriptorPool descriptorPool;
	std::vector<VkDescriptorSet> descriptorSets;
};
