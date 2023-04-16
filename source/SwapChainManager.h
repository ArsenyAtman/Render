#pragma once

#include <vector>

#include <vulkan/vulkan.h>

struct SwapChainSupportDetails
{
	VkSurfaceCapabilitiesKHR capabilities;
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR> presentModes;
};

class Window;
struct QueueFamilyIndices;
class DepthBuffer;

class SwapChainManager
{

public:

	SwapChainManager(VkDevice logicalDevice, VkPhysicalDevice physicalDevice, VkQueue graphicsQueue, VkCommandPool commandPool, const QueueFamilyIndices& indices, VkSurfaceKHR surface, Window* window);
	virtual ~SwapChainManager();

	void recreateSwapChain();

	VkSwapchainKHR swapChain;
	std::vector<VkImage> swapChainImages;
	VkFormat swapChainImageFormat;
	VkExtent2D swapChainExtent;
	std::vector<VkImageView> swapChainImageViews;
	std::vector<VkFramebuffer> swapChainFramebuffers;

	VkRenderPass renderPass;

	DepthBuffer* depthBuffer;

private:

	void createSwapChain(VkDevice logicalDevice, VkPhysicalDevice physicalDevice, VkQueue graphicsQueue, VkCommandPool commandPool, const QueueFamilyIndices& indices, VkSurfaceKHR surface);
	void createImageViews();
	void createRenderPass(VkPhysicalDevice physicalDevice);
	void createFramebuffer(VkImageView depthImageView);

	VkDevice logicalDevice;
	Window* window;

	SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface);

	VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
	VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
	VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
};