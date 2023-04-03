#include "WindowManager.h"

#include <stdexcept>

WindowManager::WindowManager(uint32_t width, uint32_t height, const char* name, VkInstance instance)
{
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); // Tell GLFW not to create OpenGL context.
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE); // Disable resizing.
	window = glfwCreateWindow(width, height, name, nullptr, nullptr);

	VkResult result = glfwCreateWindowSurface(instance, window, nullptr, &surface);
	if (result != VK_SUCCESS)
	{
		throw std::runtime_error("Unable to create a window surface!");
	}

	extent = { static_cast<uint32_t>(width), static_cast<uint32_t>(height) };

	this->instance = instance;
}

WindowManager::~WindowManager()
{
	vkDestroySurfaceKHR(instance, surface, nullptr);
	glfwDestroyWindow(window);
	glfwTerminate();
}

void WindowManager::tick()
{
	glfwPollEvents();
}

bool WindowManager::isClosed() const
{
	return glfwWindowShouldClose(window);
}

VkSurfaceKHR WindowManager::getSurface() const
{
	return surface;
}

VkExtent2D WindowManager::getExtent() const
{
	return extent;
}