#pragma once

#include <cstdint>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

struct ApplicationSettings;

class SystemWindow
{
public:

	SystemWindow(const ApplicationSettings* settings);
	virtual ~SystemWindow();

	virtual void tick();

	bool isClosed() const;
	const char** getRequiredExtensions(uint32_t* outCount) const;

	VkExtent2D getExtent() const { return extent; }

	GLFWwindow* getWindow() const { return window; }

private:

	GLFWwindow* window = nullptr;
	VkExtent2D extent;
};
