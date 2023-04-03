#pragma once

#include <cstdint>

#define GLFW_INCLUDE_VULKAN // Include Vulkan inside GLFW
#include <GLFW/glfw3.h>

class WindowManager
{

public:

	WindowManager(uint32_t width, uint32_t height, const char* name);
	virtual ~WindowManager();

	virtual void tick();

	bool isClosed() const;
	VkExtent2D getExtent() const { return extent; }
	const char** getRequiredExtensions(uint32_t* outCount) const;

	GLFWwindow* getWindow() const { return window; }

private:

	GLFWwindow* window = nullptr;
	VkExtent2D extent;
};

