#pragma once

#include "RenderModule.h"

#include <vector>

#include <vulkan/vulkan.h>

class Window;
struct QueueFamilyIndices;
class DepthBuffer;

struct SwapChainSupportDetails
{
	VkSurfaceCapabilitiesKHR capabilities;
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR> presentModes;
};

class SwapChain : public RenderModule
{
public:

	SwapChain(Render* render, Device* device, const ApplicationSettings* settings, Window* window);
	virtual ~SwapChain();

	//TODO: void recreateSwapChain();

	const VkSwapchainKHR& getSwapChain() const { return swapChain; }
	const VkRenderPass& getRenderPass() const { return renderPass; }
	const VkExtent2D& getSwapChainExtent() const { return swapChainExtent; }

	VkRenderPassBeginInfo getRenderPassInfo(uint32_t imageIndex) const;
	VkViewport getViewport() const;
	VkRect2D getScissor() const;

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

	VkSwapchainKHR swapChain;
	std::vector<VkImage> swapChainImages;
	VkFormat swapChainImageFormat;
	VkExtent2D swapChainExtent;
	std::vector<VkImageView> swapChainImageViews;
	std::vector<VkFramebuffer> swapChainFramebuffers;

	VkRenderPass renderPass;

	DepthBuffer* depthBuffer;

	std::vector<VkClearValue> renderPassClearVaules;
};
