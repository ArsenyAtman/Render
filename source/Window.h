#pragma once

#include <vulkan/vulkan.h>

struct ApplicationSettings;
class SystemWindow;
class Instance;
class Surface;

class Window
{
public:

	Window(const ApplicationSettings* settings);
	virtual ~Window();

	VkExtent2D getActualExtent() const;
	VkSurfaceKHR getSurface() const;
	VkInstance getInstance() const;

	bool shouldExit() const;
	void tick();

private:

	SystemWindow* systemWindow = nullptr;
	Instance* instance = nullptr;
	Surface* surface = nullptr;
};

