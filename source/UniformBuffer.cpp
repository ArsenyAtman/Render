#include "UniformBuffer.h"

#include <stdexcept>
#include <chrono>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Helpers.h"
#include "Settings.h"

UniformBuffer::UniformBuffer(VkDevice logicalDevice, VkPhysicalDevice physicalDevice, const ApplicationSettings* settings)
{
	this->logicalDevice = logicalDevice;
	this->settings = settings;

	VkDeviceSize bufferSize = sizeof(UniformBufferObject);

	uniformBuffers.resize(settings->maxFramesInFlight);
	uniformBuffersMemory.resize(settings->maxFramesInFlight);
	uniformBuffersMapped.resize(settings->maxFramesInFlight);

	for (size_t i = 0; i < settings->maxFramesInFlight; i++)
	{
		Helpers::createBuffer(logicalDevice, physicalDevice, bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, uniformBuffers[i], uniformBuffersMemory[i]);
		vkMapMemory(logicalDevice, uniformBuffersMemory[i], 0, bufferSize, 0, &uniformBuffersMapped[i]);
	}

	uboLayoutBinding = VkDescriptorSetLayoutBinding();
	uboLayoutBinding.binding = 0;
	uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	uboLayoutBinding.descriptorCount = 1;
	uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
}

UniformBuffer::~UniformBuffer()
{
	for (size_t i = 0; i < uniformBuffers.size(); ++i)
	{
		vkDestroyBuffer(logicalDevice, uniformBuffers[i], nullptr);
	}

	for (size_t i = 0; i < uniformBuffersMemory.size(); ++i)
	{
		vkFreeMemory(logicalDevice, uniformBuffersMemory[i], nullptr);
	}
}

void UniformBuffer::update(uint32_t currentFrame, VkExtent2D extent)
{
	static std::chrono::steady_clock::time_point startTime = std::chrono::high_resolution_clock::now();

	std::chrono::steady_clock::time_point currentTime = std::chrono::high_resolution_clock::now();
	float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

	glm::vec3 forward = glm::vec3(1.0f, 0.0f, 0.0f);
	glm::vec3 right = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 up = glm::vec3(0.0f, 0.0f, 1.0f);

	glm::vec3 viewPoint = glm::vec3(settings->viewPoint[0], settings->viewPoint[1], settings->viewPoint[2]);
	glm::vec3 viewTarget = glm::vec3(settings->viewTarget[0], settings->viewTarget[1], settings->viewTarget[2]);

	glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), glm::radians(settings->rotation[0]), forward) * glm::rotate(glm::mat4(1.0f), glm::radians(settings->rotation[1]), right) * glm::rotate(glm::mat4(1.0f), glm::radians(settings->rotation[2]), up);

	UniformBufferObject ubo{};
	ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(settings->rotationSpeed), up) * rotation;
	ubo.view = glm::lookAt(viewPoint, viewTarget, up);
	ubo.proj = glm::perspective(glm::radians(settings->fov), static_cast<float>(extent.width) / static_cast<float>(extent.height), settings->perspectiveNear, settings->perspectiveFar);
	ubo.proj[1][1] *= -1;

	memcpy(uniformBuffersMapped[currentFrame], &ubo, sizeof(ubo));
}
