#include "DescriptorsManager.h"

#include <stdexcept>
#include <array>

#include "Render.h"
#include "Device.h"
#include "Settings.h"
#include "Model.h"
#include "UniformBuffer.h"
#include "TextureBuffer.h"

DescriptorsManager::DescriptorsManager(Render* render, Device* device, const ApplicationSettings* settings, const Model* model) : RenderModule(render, device, settings)
{
	for (uint32_t i = 0; i < settings->maxFramesInFlight; ++i)
	{
		TextureBuffer* textureBuffer = new TextureBuffer(render, device, settings, model->getTexture());
		UniformBuffer* uniformBuffer = new UniformBuffer(render, device, settings);
		
		std::vector<Descriptor*> descriptorsForFrame = { textureBuffer, uniformBuffer };

		descriptorsForFrames.push_back(descriptorsForFrame);
	}

	createDescriptorSetLayout();
	createDescriptorPool();
	createDescriptorSets();
}

DescriptorsManager::~DescriptorsManager()
{
	vkDestroyDescriptorPool(getDevice()->getLogicalDevice(), descriptorPool, nullptr);
	vkDestroyDescriptorSetLayout(getDevice()->getLogicalDevice(), descriptorSetLayout, nullptr);

	for (std::vector<Descriptor*> descriptorsForFrame : descriptorsForFrames)
	{
		for (Descriptor* descriptor : descriptorsForFrame)
		{
			delete descriptor;
		}
	}
}

void DescriptorsManager::tick()
{
	for (std::vector<Descriptor*> descriptorsForFrame : descriptorsForFrames)
	{
		for (Descriptor* descriptor : descriptorsForFrame)
		{
			descriptor->tick();
		}
	}
}

const VkDescriptorSetLayout& DescriptorsManager::getDescriptorSetLayout() const
{
	return descriptorSetLayout;
}

const VkDescriptorSet& DescriptorsManager::getDescriptorSetForCurrentFrame() const
{
	return descriptorSets[getRender()->getCurrentFrame()];
}

void DescriptorsManager::createDescriptorSetLayout()
{
	std::vector<VkDescriptorSetLayoutBinding> bindings;
	for (const Descriptor* descriptor : descriptorsForFrames[0])
	{
		bindings.push_back(descriptor->getLayoutBinding());
	}
	
	VkDescriptorSetLayoutCreateInfo layoutInfo{};
	layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
	layoutInfo.pBindings = bindings.data();

	VkResult result = vkCreateDescriptorSetLayout(getDevice()->getLogicalDevice(), &layoutInfo, nullptr, &descriptorSetLayout);
	if (result != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to create a descriptor set layout!");
	}
}

void DescriptorsManager::createDescriptorPool()
{
	std::vector<VkDescriptorPoolSize> poolSizes;
	for (const Descriptor* descriptor : descriptorsForFrames[0])
	{
		poolSizes.push_back(descriptor->getPoolSize());
	}

	VkDescriptorPoolCreateInfo poolInfo{};
	poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
	poolInfo.pPoolSizes = poolSizes.data();
	poolInfo.maxSets = static_cast<uint32_t>(descriptorsForFrames.size());

	VkResult result = vkCreateDescriptorPool(getDevice()->getLogicalDevice(), &poolInfo, nullptr, &descriptorPool);
	if (result != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to create a descriptor pool!");
	}
}

void DescriptorsManager::createDescriptorSets()
{
	std::vector<VkDescriptorSetLayout> layouts(getSettings()->maxFramesInFlight, descriptorSetLayout);
	
	VkDescriptorSetAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = descriptorPool;
	allocInfo.descriptorSetCount = static_cast<uint32_t>(getSettings()->maxFramesInFlight);
	allocInfo.pSetLayouts = layouts.data();

	descriptorSets.resize(getSettings()->maxFramesInFlight);

	VkResult result = vkAllocateDescriptorSets(getDevice()->getLogicalDevice(), &allocInfo, descriptorSets.data());
	if (result != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to allocate descriptor sets!");
	}

	for (size_t i = 0; i < descriptorsForFrames.size(); ++i)
	{
		std::vector<Descriptor*>& descriptorsForFrame = descriptorsForFrames[i];

		std::vector<VkWriteDescriptorSet> descriptorWrites;
		for (const Descriptor* descriptor : descriptorsForFrame)
		{
			descriptorWrites.push_back(descriptor->getWriteSet(descriptorSets[i]));
		}

		vkUpdateDescriptorSets(getDevice()->getLogicalDevice(), static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
	}
}
