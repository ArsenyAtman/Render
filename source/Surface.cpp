#include "Surface.h"

#include <stdexcept>

#include "SystemWindow.h"
#include "Instance.h"

Surface::Surface(SystemWindow* systemWindow, Instance* instance)
{
	VkResult result = glfwCreateWindowSurface(instance->getInstance(), systemWindow->getWindow(), nullptr, &surface);
	if (result != VK_SUCCESS)
	{
		throw std::runtime_error("Unable to create surface!");
	}

	this->instance = instance;
}

Surface::~Surface()
{
	vkDestroySurfaceKHR(instance->getInstance(), surface, nullptr);
}
