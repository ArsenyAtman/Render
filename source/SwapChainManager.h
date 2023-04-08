#pragma once

#include <vector>

#define GLFW_INCLUDE_VULKAN // Include Vulkan inside GLFW
#include <GLFW/glfw3.h>

struct SwapChainSupportDetails
{
	VkSurfaceCapabilitiesKHR capabilities;
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR> presentModes;
};

class WindowManager;
struct QueueFamilyIndices;

class SwapChainManager
{

public:

	SwapChainManager(VkPhysicalDevice physicalDevice, const QueueFamilyIndices& indices, VkSurfaceKHR surface, VkDevice logicalDevice, WindowManager* windowManager);
	virtual ~SwapChainManager();

	VkSwapchainKHR swapChain;
	std::vector<VkImage> swapChainImages;
	VkFormat swapChainImageFormat;
	VkExtent2D swapChainExtent;
	std::vector<VkImageView> swapChainImageViews;
	std::vector<VkFramebuffer> swapChainFramebuffers;

	VkRenderPass renderPass;

private:

	void createSwapChain(VkPhysicalDevice physicalDevice, const QueueFamilyIndices& indices, VkSurfaceKHR surface);
	void createImageViews();
	void createRenderPass();
	void createFramebuffer();

	VkDevice logicalDevice;
	WindowManager* windowManager;

	SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface);

	VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
	VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
	VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
	void createSwapChain();
};