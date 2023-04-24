#include "UniformBuffer.h"

#include <stdexcept>
#include <chrono>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Helpers.h"
#include "Render.h"
#include "Device.h"
#include "Settings.h"

#include "SwapChain.h"

UniformBuffer::UniformBuffer(Render* render, Device* device, const ApplicationSettings* settings) : Descriptor(render, device, settings)
{
	VkDeviceSize bufferSize = sizeof(UniformBufferObject);

	Helpers::createBuffer(getDevice(), bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, uniformBuffer, uniformBufferMemory);
	vkMapMemory(getDevice()->getLogicalDevice(), uniformBufferMemory, 0, bufferSize, 0, &uniformBufferMapped);

	bufferInfo = VkDescriptorBufferInfo();
	bufferInfo.buffer = uniformBuffer;
	bufferInfo.offset = 0;
	bufferInfo.range = sizeof(UniformBufferObject);
}

UniformBuffer::~UniformBuffer()
{
	vkDestroyBuffer(getDevice()->getLogicalDevice(), uniformBuffer, nullptr);
	vkFreeMemory(getDevice()->getLogicalDevice(), uniformBufferMemory, nullptr);
}

void UniformBuffer::tick()
{
	static std::chrono::steady_clock::time_point startTime = std::chrono::high_resolution_clock::now();

	std::chrono::steady_clock::time_point currentTime = std::chrono::high_resolution_clock::now();
	float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

	glm::vec3 forward = glm::vec3(1.0f, 0.0f, 0.0f);
	glm::vec3 right = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 up = glm::vec3(0.0f, 0.0f, 1.0f);

	glm::vec3 viewPoint = glm::vec3(getSettings()->viewPoint[0], getSettings()->viewPoint[1], getSettings()->viewPoint[2]);
	glm::vec3 viewTarget = glm::vec3(getSettings()->viewTarget[0], getSettings()->viewTarget[1], getSettings()->viewTarget[2]);

	glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), glm::radians(getSettings()->rotation[0]), forward) * glm::rotate(glm::mat4(1.0f), glm::radians(getSettings()->rotation[1]), right) * glm::rotate(glm::mat4(1.0f), glm::radians(getSettings()->rotation[2]), up);

	const VkExtent2D& extent = getRender()->getSwapChain()->getSwapChainExtent();

	UniformBufferObject ubo{};
	ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(getSettings()->rotationSpeed), up) * rotation;
	ubo.view = glm::lookAt(viewPoint, viewTarget, up);
	ubo.proj = glm::perspective(glm::radians(getSettings()->fov), static_cast<float>(extent.width) / static_cast<float>(extent.height), getSettings()->perspectiveNear, getSettings()->perspectiveFar);
	ubo.proj[1][1] *= -1;

	memcpy(uniformBufferMapped, &ubo, sizeof(ubo));
}

VkDescriptorSetLayoutBinding UniformBuffer::getLayoutBinding() const
{
	VkDescriptorSetLayoutBinding layoutBinding{};
	layoutBinding.binding = 0;
	layoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	layoutBinding.descriptorCount = 1;
	layoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

	return layoutBinding;
}

VkDescriptorPoolSize UniformBuffer::getPoolSize() const
{
	VkDescriptorPoolSize poolSize{};
	poolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	poolSize.descriptorCount = static_cast<uint32_t>(getSettings()->maxFramesInFlight);

	return poolSize;
}

VkWriteDescriptorSet UniformBuffer::getWriteSet(VkDescriptorSet descriptorSet) const
{
	VkWriteDescriptorSet writeSet{};
	writeSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	writeSet.dstSet = descriptorSet;
	writeSet.dstBinding = 0;
	writeSet.dstArrayElement = 0;
	writeSet.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	writeSet.descriptorCount = 1;
	writeSet.pBufferInfo = &bufferInfo;

	return writeSet;
}
