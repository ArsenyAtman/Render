#pragma once

#include "RenderModule.h"
#include "Tickable.h"

#include <vulkan/vulkan.h>

class Descriptor : public RenderModule, public Tickable
{
public:

	Descriptor(Render* render, Device* device, const ApplicationSettings* settings);
	virtual ~Descriptor();

	virtual void tick() override { return; }

	virtual VkDescriptorSetLayoutBinding getLayoutBinding() const = 0;
	virtual VkDescriptorPoolSize getPoolSize() const = 0;
	virtual VkWriteDescriptorSet getWriteSet(VkDescriptorSet descriptorSet) const = 0;
};
