#include "SystemWindow.h"

#include <stdexcept>

#include "Settings.h"

SystemWindow::SystemWindow(const ApplicationSettings* settings)
{
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); // Tell GLFW not to create OpenGL context.
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE); // Disable resizing.
	window = glfwCreateWindow(settings->windowWidth, settings->windowHeight, settings->windowName.data(), nullptr, nullptr);

	extent = { settings->windowWidth, settings->windowHeight };
}

SystemWindow::~SystemWindow()
{
	glfwDestroyWindow(window);
	glfwTerminate();
}

void SystemWindow::tick()
{
	glfwPollEvents();
}

bool SystemWindow::isClosed() const
{
	return glfwWindowShouldClose(window);
}

const char** SystemWindow::getRequiredExtensions(uint32_t* outCount) const
{
	return glfwGetRequiredInstanceExtensions(outCount);
}
