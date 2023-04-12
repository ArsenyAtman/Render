#pragma once

#include <vector>

#include <vulkan/vulkan.h>

const bool enableValidationLayers = false;

const std::vector<const char*> validationLayers = { "VK_LAYER_KHRONOS_validation" };
const std::vector<const char*> deviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

class WindowManager;
class SurfaceManager;
class InstanceManager;
class PhysicalDeviceManager;
class LogicalDeviceManager;
class VertexBuffer;
class UniformBuffer;
class SwapChainManager;
class GraphicsPipeline;
class CommandManager;
class SyncsManager;
class TextureImage;
class DescriptorsManager;
class ModelLoader;

class Render
{
public:

	Render();

private:

	WindowManager* windowManager = nullptr;
	SurfaceManager* surfaceManager = nullptr;
	InstanceManager* instanceManager = nullptr;
	PhysicalDeviceManager* physicalDeviceManager = nullptr;
	LogicalDeviceManager* logicalDeviceManager = nullptr;
	VertexBuffer* vertexBuffer = nullptr;
	UniformBuffer* uniformBuffer = nullptr;
	TextureImage* textureImage = nullptr;
	DescriptorsManager* descriptorsManager = nullptr;
	SwapChainManager* swapChainManager = nullptr;
	GraphicsPipeline* graphicsPipeline = nullptr;
	CommandManager* commandManager = nullptr;
	SyncsManager* syncsManager = nullptr;
	ModelLoader* modelLoader = nullptr;

	void drawFrame();
};
