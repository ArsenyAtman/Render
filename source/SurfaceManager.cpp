#include "SurfaceManager.h"

#include <stdexcept>

SurfaceManager::SurfaceManager(VkInstance instance, GLFWwindow* window)
{
	VkResult result = glfwCreateWindowSurface(instance, window, nullptr, &surface);
	if (result != VK_SUCCESS)
	{
		throw std::runtime_error("Unable to create a window surface!");
	}

	this->instance = instance;
}

SurfaceManager::~SurfaceManager()
{
	vkDestroySurfaceKHR(instance, surface, nullptr);
}