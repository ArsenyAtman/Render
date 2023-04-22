#pragma once

#include <vector>

#include <glm/glm.hpp>

#include <vulkan/vulkan.h>

struct ApplicationSettings;

struct UniformBufferObject
{
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 proj;
};

class UniformBuffer
{

public:

    UniformBuffer(VkDevice logicalDevice, VkPhysicalDevice physicalDevice, const ApplicationSettings* settings);
    virtual ~UniformBuffer();

    void update(uint32_t currentFrame, VkExtent2D extent);

    std::vector<VkBuffer> uniformBuffers;
    std::vector<VkDeviceMemory> uniformBuffersMemory;
    std::vector<void*> uniformBuffersMapped;

    VkDescriptorSetLayoutBinding uboLayoutBinding;

private:

    VkDevice logicalDevice;
    const ApplicationSettings* settings;
};
