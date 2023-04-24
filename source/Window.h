#pragma once

#include "Tickable.h"

#include <vulkan/vulkan.h>

struct ApplicationSettings;
class SystemWindow;
class Instance;
class Surface;

class Window : public Tickable
{
public:

	Window(const ApplicationSettings* settings);
	virtual ~Window();

	VkExtent2D getActualExtent() const;
	VkSurfaceKHR getSurface() const;
	VkInstance getInstance() const;

	bool shouldExit() const;
	
	virtual void tick() override;

private:

	SystemWindow* systemWindow = nullptr;
	Instance* instance = nullptr;
	Surface* surface = nullptr;
};
