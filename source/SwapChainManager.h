#pragma once

#include "RenderModule.h"

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

class SwapChainManager : public RenderModule
{
public:

	SwapChainManager(Render* render, Device* device, const ApplicationSettings* settings, Window* window);
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

	void createSwapChain();
	void createImageViews();
	void createRenderPass();
	void createFramebuffer();

	SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface);

	VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
	VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
	VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

	Window* window;
};