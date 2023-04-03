#include "WindowManager.h"

#include <stdexcept>

WindowManager::WindowManager(uint32_t width, uint32_t height, const char* name)
{
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); // Tell GLFW not to create OpenGL context.
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE); // Disable resizing.
	window = glfwCreateWindow(width, height, name, nullptr, nullptr);

	extent = { static_cast<uint32_t>(width), static_cast<uint32_t>(height) };
}

WindowManager::~WindowManager()
{
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

const char** WindowManager::getRequiredExtensions(uint32_t* outCount) const
{
	return glfwGetRequiredInstanceExtensions(outCount);
}
