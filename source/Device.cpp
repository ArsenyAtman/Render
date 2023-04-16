#include "Device.h"

#include <stdexcept>
#include <set>
#include <string>

#include "Window.h"
#include "Settings.h"

Device::Device(Window* window, const ApplicationSettings* settings)
{
	physicalDevice = pickPhysicalDevice(window, settings);
	if (physicalDevice == VK_NULL_HANDLE)
	{
		throw std::runtime_error("Failed to pick physical device!");
	}

	getQueueIndices(physicalDevice, window, queueIndices);

	logicalDevice = createLogicalDevice(physicalDevice, queueIndices, settings);
	if (physicalDevice == VK_NULL_HANDLE)
	{
		throw std::runtime_error("Failed to create logical device!");
	}

	getQueues(logicalDevice, queueIndices, graphicsQueue, presentQueue);
}

Device::~Device()
{
	vkDestroyDevice(logicalDevice, nullptr);
}

VkPhysicalDevice Device::pickPhysicalDevice(Window* window, const ApplicationSettings* settings) const
{
	uint32_t countOfDevices = 0;
	vkEnumeratePhysicalDevices(window->getInstance(), &countOfDevices, nullptr);

	if (countOfDevices == 0)
	{
		throw std::runtime_error("Failed to find GPUs with Vulkan support!");
	}

	std::vector<VkPhysicalDevice> devices(countOfDevices);
	vkEnumeratePhysicalDevices(window->getInstance(), &countOfDevices, devices.data());

	for (const VkPhysicalDevice& device : devices)
	{
		if (checkDeviceRequirements(device, window, settings))
		{
			return device;
		}
	}

	return VK_NULL_HANDLE;
}

bool Device::checkDeviceRequirements(VkPhysicalDevice device, Window* window, const ApplicationSettings* settings) const
{
	bool queuesSupport = supportsQueues(device, window);
	bool extensionsSupport = supportsExtensions(device, window, settings);
	bool swapChainSupport = supportsSwapChain(device, window);
	bool featuresSupport = supportsFeatures(device, window);

	return queuesSupport && extensionsSupport && swapChainSupport && featuresSupport;
}

bool Device::supportsQueues(VkPhysicalDevice device, Window* window) const
{
	uint32_t countOfQueueFamilies = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(device, &countOfQueueFamilies, nullptr);

	std::vector<VkQueueFamilyProperties> queueFamilies(countOfQueueFamilies);
	vkGetPhysicalDeviceQueueFamilyProperties(device, &countOfQueueFamilies, queueFamilies.data());

	bool supportsGraphicsQueue = false;
	bool supportsPresentQueue = false;
	for (int i = 0; i < queueFamilies.size(); ++i)
	{
		const VkQueueFamilyProperties& queueFamily = queueFamilies[i];

		if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
		{
			supportsGraphicsQueue = true;
		}

		VkBool32 supportOfPresentQueue = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(device, i, window->getSurface(), &supportOfPresentQueue);
		if (supportOfPresentQueue)
		{
			supportsPresentQueue = true;
		}

		if (supportsGraphicsQueue && supportsPresentQueue)
		{
			return true;
		}
	}

	return false;
}

bool Device::supportsExtensions(VkPhysicalDevice device, Window* window, const ApplicationSettings* settings) const
{
	uint32_t countOfExtensions;
	vkEnumerateDeviceExtensionProperties(device, nullptr, &countOfExtensions, nullptr);

	std::vector<VkExtensionProperties> availableExtensions(countOfExtensions);
	vkEnumerateDeviceExtensionProperties(device, nullptr, &countOfExtensions, availableExtensions.data());

	std::set<std::string> requiredExtensions(settings->deviceExtensions.begin(), settings->deviceExtensions.end());

	for (const auto& extension : availableExtensions)
	{
		requiredExtensions.erase(extension.extensionName);
	}

	return requiredExtensions.empty();
}

bool Device::supportsSwapChain(VkPhysicalDevice device, Window* window) const
{
	uint32_t countOfFormats;
	vkGetPhysicalDeviceSurfaceFormatsKHR(device, window->getSurface(), &countOfFormats, nullptr);

	uint32_t countOfPresentModes;
	vkGetPhysicalDeviceSurfacePresentModesKHR(device, window->getSurface(), &countOfPresentModes, nullptr);

	return (countOfFormats > 0) && (countOfPresentModes > 0);
}

bool Device::supportsFeatures(VkPhysicalDevice device, Window* window) const
{
	VkPhysicalDeviceFeatures supportedFeatures;
	vkGetPhysicalDeviceFeatures(device, &supportedFeatures);
	return supportedFeatures.samplerAnisotropy;
}

void Device::getQueueIndices(VkPhysicalDevice device, Window* window, QueueFamilyIndices& outIndicies) const
{
	uint32_t countOfQueueFamilies = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(device, &countOfQueueFamilies, nullptr);

	std::vector<VkQueueFamilyProperties> queueFamilies(countOfQueueFamilies);
	vkGetPhysicalDeviceQueueFamilyProperties(device, &countOfQueueFamilies, queueFamilies.data());

	for (int i = 0; i < queueFamilies.size(); ++i)
	{
		const VkQueueFamilyProperties& queueFamily = queueFamilies[i];
		if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
		{
			outIndicies.graphicsFamily = i;
		}

		VkBool32 presentSupport = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(device, i, window->getSurface(), &presentSupport);
		if (presentSupport)
		{
			outIndicies.presentFamily = i;
		}

		if (outIndicies.isComplete())
		{
			return;
		}
	}
}

VkDevice Device::createLogicalDevice(VkPhysicalDevice physicalDevice, const QueueFamilyIndices& indices, const ApplicationSettings* settings) const
{
	std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
	std::set<uint32_t> queueIndices = { indices.graphicsFamily.value(), indices.presentFamily.value() };

	float queuePriority = 1.0f;
	for (uint32_t queueIndex : queueIndices)
	{
		VkDeviceQueueCreateInfo queueCreateInfo = VkDeviceQueueCreateInfo();
		queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueCreateInfo.queueFamilyIndex = queueIndex;
		queueCreateInfo.queueCount = 1;
		queueCreateInfo.pQueuePriorities = &queuePriority;
		queueCreateInfos.push_back(queueCreateInfo);
	}

	VkPhysicalDeviceFeatures deviceFeatures = VkPhysicalDeviceFeatures();
	deviceFeatures.samplerAnisotropy = VK_TRUE;

	VkDeviceCreateInfo createInfo = VkDeviceCreateInfo();
	createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	createInfo.pQueueCreateInfos = queueCreateInfos.data();
	createInfo.queueCreateInfoCount = 1;
	createInfo.pEnabledFeatures = &deviceFeatures;
	createInfo.enabledExtensionCount = static_cast<uint32_t>(settings->deviceExtensions.size());
	createInfo.ppEnabledExtensionNames = settings->deviceExtensions.data();
	if (settings->withValidationLayers)
	{
		createInfo.enabledLayerCount = static_cast<uint32_t>(settings->validationLayers.size());
		createInfo.ppEnabledLayerNames = settings->validationLayers.data();
	}
	else
	{
		createInfo.enabledLayerCount = 0;
	}

	VkDevice device;
	VkResult result = vkCreateDevice(physicalDevice, &createInfo, nullptr, &device);
	if (result != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to create a logical device!");
	}

	return device;
}

void Device::getQueues(VkDevice logicalDevice, const QueueFamilyIndices& indices, VkQueue& outGraphicsQueue, VkQueue& outPresentQueue) const
{
	vkGetDeviceQueue(logicalDevice, indices.graphicsFamily.value(), 0, &outGraphicsQueue);
	vkGetDeviceQueue(logicalDevice, indices.presentFamily.value(), 0, &outPresentQueue);
}
