#include "Window.h"

#include "Settings.h"
#include "SystemWindow.h"
#include "Instance.h"
#include "Surface.h"

Window::Window(const ApplicationSettings* settings)
{
	systemWindow = new SystemWindow(settings);
	instance = new Instance(systemWindow, settings);
	surface = new Surface(systemWindow, instance);
}

Window::~Window()
{
	delete surface;
	delete instance;
	delete systemWindow;
}

VkExtent2D Window::getActualExtent() const
{
	return systemWindow->getExtent();
}

VkSurfaceKHR Window::getSurface() const
{
	return surface->getSurface();
}

VkInstance Window::getInstance() const
{
	return instance->getInstance();
}

bool Window::shouldExit() const
{
	return systemWindow->isClosed();
}

void Window::tick()
{
	systemWindow->tick();
}
