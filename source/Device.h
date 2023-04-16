#pragma once

#include <vulkan/vulkan.h>

#include <vector>
#include <optional>
#include <cstdint>

struct ApplicationSettings;
class Window;

struct QueueFamilyIndices
{
	std::optional<uint32_t> graphicsFamily;
	std::optional<uint32_t> presentFamily;

	bool isComplete()
	{
		return graphicsFamily.has_value() && presentFamily.has_value();
	}
};

class Device
{
public:

	Device(Window* window, const ApplicationSettings* settings);
	virtual ~Device();

	VkPhysicalDevice getPhysicalDevice() const { return physicalDevice; }
	QueueFamilyIndices getQueueIndices() const { return queueIndices; }

	VkDevice getLogicalDevice() const { return logicalDevice; }
	VkQueue getGraphicsQueue() const { return graphicsQueue; }
	VkQueue getPresentQueue() const { return presentQueue; }

protected:

	VkPhysicalDevice pickPhysicalDevice(Window* window, const ApplicationSettings* settings) const;

	bool checkDeviceRequirements(VkPhysicalDevice device, Window* window, const ApplicationSettings* settings) const;

	bool supportsQueues(VkPhysicalDevice device, Window* window) const;
	bool supportsExtensions(VkPhysicalDevice device, Window* window, const ApplicationSettings* settings)const;
	bool supportsSwapChain(VkPhysicalDevice device, Window* window) const;
	bool supportsFeatures(VkPhysicalDevice device, Window* window) const;

	void getQueueIndices(VkPhysicalDevice device, Window* window, QueueFamilyIndices& outIndicies) const;

	VkDevice createLogicalDevice(VkPhysicalDevice physicalDevice, const QueueFamilyIndices& indices, const ApplicationSettings* settings) const;
	void getQueues(VkDevice logicalDevice, const QueueFamilyIndices& indices, VkQueue& outGraphicsQueue, VkQueue& outPresentQueue) const;

private:

	VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
	QueueFamilyIndices queueIndices;

	VkDevice logicalDevice = VK_NULL_HANDLE;
	VkQueue graphicsQueue;
	VkQueue presentQueue;
};
