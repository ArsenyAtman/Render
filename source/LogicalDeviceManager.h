#pragma once

#define GLFW_INCLUDE_VULKAN // Include Vulkan inside GLFW
#include <GLFW/glfw3.h>

#include "PhysicalDeviceManager.h"

class LogicalDeviceManager
{

public:

	LogicalDeviceManager(VkPhysicalDevice physicalDevice, const QueueFamilyIndices& indices, bool enableValidationLayers, const std::vector<const char*>& validationLayers, const std::vector<const char*>& deviceExtensions);
	virtual ~LogicalDeviceManager();

	VkDevice getDevice() const { return device; }
	VkQueue getGraphicsQueue() const { return graphicsQueue; }
	VkQueue getPresentQueue() const { return presentQueue; }

private:

	VkDevice device;
	VkQueue graphicsQueue;
	VkQueue presentQueue;
};

