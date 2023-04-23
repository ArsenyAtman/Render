#pragma once

#include "Descriptor.h"

#include <vector>

#include <glm/glm.hpp>
#include <vulkan/vulkan.h>

struct UniformBufferObject
{
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 proj;
};

class UniformBuffer : public Descriptor
{
public:

    UniformBuffer(Render* render, Device* device, const ApplicationSettings* settings);
    virtual ~UniformBuffer();

    virtual void tick() override;

    virtual VkDescriptorSetLayoutBinding getLayoutBinding() const override;
    virtual VkDescriptorPoolSize getPoolSize() const override;
    virtual VkWriteDescriptorSet getWriteSet(VkDescriptorSet descriptorSet) const override;

private:

    VkBuffer uniformBuffer;
    VkDeviceMemory uniformBufferMemory;
    void* uniformBufferMapped;

    VkDescriptorBufferInfo bufferInfo;
};
