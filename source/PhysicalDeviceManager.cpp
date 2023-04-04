#include "PhysicalDeviceManager.h"

#include <stdexcept>
#include <vector>
#include <set>
#include <string>

PhysicalDeviceManager::PhysicalDeviceManager(VkInstance instance, VkSurfaceKHR surface, const std::vector<const char*>& deviceExtensions)
{
	uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

	if (deviceCount == 0)
	{
		throw std::runtime_error("Failed to find GPUs with Vulkan support!");
	}

	std::vector<VkPhysicalDevice> devices(deviceCount);
	vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

	for (const VkPhysicalDevice& device : devices)
	{
		if (isDeviceSuitable(device, surface, deviceExtensions))
		{
			physicalDevice = device;
			queueFamilyIndices = findQueueFamilies(device, surface);
			break;
		}
	}

	if (physicalDevice == VK_NULL_HANDLE)
	{
		throw std::runtime_error("Failed to find a suitable GPU!");
	}
}

PhysicalDeviceManager::~PhysicalDeviceManager()
{
	// ...
}

bool PhysicalDeviceManager::isDeviceSuitable(VkPhysicalDevice device, VkSurfaceKHR surface, const std::vector<const char*>& deviceExtensions)
{
	QueueFamilyIndices indices = findQueueFamilies(device, surface);
	bool extensionsSupported = checkDeviceExtensionSupport(device, deviceExtensions);
	bool swapChainAdequate = supportsSwapChain(device, surface);
	return indices.isComplete() && extensionsSupported && swapChainAdequate;
}

bool PhysicalDeviceManager::checkDeviceExtensionSupport(VkPhysicalDevice device, const std::vector<const char*>& deviceExtensions)
{
	uint32_t extensionCount;
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

	std::vector<VkExtensionProperties> availableExtensions(extensionCount);
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

	std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

	for (const auto& extension : availableExtensions)
	{
		requiredExtensions.erase(extension.extensionName);
	}

	return requiredExtensions.empty();
}

bool PhysicalDeviceManager::supportsSwapChain(VkPhysicalDevice device, VkSurfaceKHR surface)
{
	uint32_t countOfFormats;
	vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &countOfFormats, nullptr);

	uint32_t countOfPresentModes;
	vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &countOfPresentModes, nullptr);

	return (countOfFormats > 0) && (countOfPresentModes > 0);
}

QueueFamilyIndices PhysicalDeviceManager::findQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface)
{
	QueueFamilyIndices indices;

	uint32_t countOfQueueFamilies = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(device, &countOfQueueFamilies, nullptr);

	std::vector<VkQueueFamilyProperties> queueFamilies(countOfQueueFamilies);
	vkGetPhysicalDeviceQueueFamilyProperties(device, &countOfQueueFamilies, queueFamilies.data());

	for (int i = 0; i < queueFamilies.size(); ++i)
	{
		const VkQueueFamilyProperties& queueFamily = queueFamilies[i];
		if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
		{
			indices.graphicsFamily = i;
		}

		VkBool32 presentSupport = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);
		if (presentSupport)
		{
			indices.presentFamily = i;
		}

		if (indices.isComplete())
		{
			break;
		}
	}

	return indices;
}
