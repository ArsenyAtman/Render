#pragma once

#define GLFW_INCLUDE_VULKAN // Include Vulkan inside GLFW
#include <GLFW/glfw3.h>

#include <vector>

class WindowManager;

class InstanceManager
{

public:

	InstanceManager(WindowManager* windowManager, bool withValidationLayers, const std::vector<const char*>& requestedValidationLayers);
	virtual ~InstanceManager();

	VkInstance getInstance() { return instance; }

private:

	bool checkValidationLayersSupport(const std::vector<const char*>& validationLayers);

	VkInstance instance;
};

