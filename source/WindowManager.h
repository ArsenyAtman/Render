#pragma once

#include <cstdint>

#define GLFW_INCLUDE_VULKAN // Include Vulkan inside GLFW
#include <GLFW/glfw3.h>

class WindowManager
{

public:

	WindowManager(uint32_t width, uint32_t height, const char* name, VkInstance instance);
	virtual ~WindowManager();

	virtual void tick();

	bool isClosed() const;

	VkSurfaceKHR getSurface() const;
	VkExtent2D getExtent() const;

private:

	GLFWwindow* window;
	VkSurfaceKHR surface;
	VkExtent2D extent;
	VkInstance instance;
};

