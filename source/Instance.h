#pragma once

#include <vector>

#include <vulkan/vulkan.h>

class SystemWindow;
struct ApplicationSettings;

class Instance
{
public:

	Instance(SystemWindow* systemWindow, const ApplicationSettings* settings);
	virtual ~Instance();

	VkInstance getInstance() const { return instance; }

private:

	VkInstance createInstance(SystemWindow* systemWindow, const ApplicationSettings* settings) const;
	bool supportsValidationLayers(const std::vector<const char*>& requestedValidationLayers) const;

	VkInstance instance;
};
