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
class GraphicsPipeline;
class CommandManager;
class SyncsManager;

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
	GraphicsPipeline* graphicsPipeline = nullptr;
	CommandManager* commandManager = nullptr;
	SyncsManager* syncsManager = nullptr;

	void drawFrame();
};
