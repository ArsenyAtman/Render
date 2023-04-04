#pragma once

#include <vector>
#include <optional>
#include <string>

#define GLFW_INCLUDE_VULKAN // Include Vulkan inside GLFW
#include <GLFW/glfw3.h>

#if NDEBUG
const bool enableValidationLayers = true;
#else
const bool enableValidationLayers = false;
#endif

const std::vector<const char*> validationLayers = { "VK_LAYER_KHRONOS_validation" };
const std::vector<const char*> deviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

class WindowManager;
class SurfaceManager;
class InstanceManager;
class PhysicalDeviceManager;
class LogicalDeviceManager;
class SwapChainManager;

class Render
{
public:

	Render();

	void run();

private:

	WindowManager* windowManager = nullptr;
	SurfaceManager* surfaceManager = nullptr;
	InstanceManager* instanceManager = nullptr;
	PhysicalDeviceManager* physicalDeviceManager = nullptr;
	LogicalDeviceManager* logicalDeviceManager = nullptr;
	SwapChainManager* swapChainManager = nullptr;
	
	std::vector<VkImageView> swapChainImageViews;
	VkPipelineLayout pipelineLayout;
	VkRenderPass renderPass;
	VkPipeline graphicsPipeline;
	std::vector<VkFramebuffer> swapChainFramebuffers;
	VkCommandPool commandPool;
	VkCommandBuffer commandBuffer;
	VkSemaphore imageAvailableSemaphore;
	VkSemaphore renderFinishedSemaphore;
	VkFence inFlightFence;

	void initVulkan();
	void deinitVulkan();

	void createImageViews();

	void createGraphicsPipeline();

	std::vector<char> readFile(const std::string& filename);

	VkShaderModule createShaderModule(const std::vector<char>& code);

	void createRenderPass();

	void createFramebuffers();

	void createCommandPool();

	void createCommandBuffer();
	void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);

	void drawFrame();

	void createSyncObjects();
};
