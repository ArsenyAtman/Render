#pragma once

#define GLFW_INCLUDE_VULKAN // Include Vulkan inside GLFW
#include <GLFW/glfw3.h>

class SurfaceManager
{

public:

	SurfaceManager(VkInstance instance, GLFWwindow* window);
	virtual ~SurfaceManager();

	VkSurfaceKHR getSurface() const { return surface; }

private:

	VkInstance instance = nullptr;
	VkSurfaceKHR surface = nullptr;
};

