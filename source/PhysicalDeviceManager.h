#pragma once

#include <optional>
#include <vector>

#define GLFW_INCLUDE_VULKAN // Include Vulkan inside GLFW
#include <GLFW/glfw3.h>

struct QueueFamilyIndices
{
	std::optional<uint32_t> graphicsFamily;
	std::optional<uint32_t> presentFamily;

	bool isComplete()
	{
		return graphicsFamily.has_value() && presentFamily.has_value();
	}
};

class PhysicalDeviceManager
{

public:

	PhysicalDeviceManager(VkInstance instance, VkSurfaceKHR surface, const std::vector<const char*>& deviceExtensions);
	virtual ~PhysicalDeviceManager();

	VkPhysicalDevice getDevice() { return physicalDevice; }
	const QueueFamilyIndices& getQueueFamilyIndices() { return queueFamilyIndices; }

private:

	VkPhysicalDevice physicalDevice;
	QueueFamilyIndices queueFamilyIndices;

	bool isDeviceSuitable(VkPhysicalDevice device, VkSurfaceKHR surface, const std::vector<const char*>& deviceExtensions);
	bool checkDeviceExtensionSupport(VkPhysicalDevice device, const std::vector<const char*>& deviceExtensions);
	bool supportsSwapChain(VkPhysicalDevice device, VkSurfaceKHR surface);

	QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface);

};
