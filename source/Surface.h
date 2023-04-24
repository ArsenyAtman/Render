#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

class SystemWindow;
class Instance;

class Surface
{
public:

	Surface(SystemWindow* systemWindow, Instance* instance);
	virtual ~Surface();

	VkSurfaceKHR getSurface() const { return surface; }

private:

	VkSurfaceKHR surface;

	Instance* instance = nullptr;

};
