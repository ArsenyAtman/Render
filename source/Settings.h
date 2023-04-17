#pragma once

#include <vulkan/vulkan.h>
#include <ini.h>

#include <string>
#include <cstdint>

struct ApplicationSettings
{
	std::string applicationName = "";

	uint32_t windowWidth = 0;
	uint32_t windowHeight = 0;

	std::string windowName = "";

	bool withValidationLayers = true;
	std::vector<const char*> validationLayers = { "VK_LAYER_KHRONOS_validation" };

	const std::vector<const char*> deviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

	std::string meshPath;
	std::string texturePath;
	std::vector<std::string> shaderPaths;
};

class Settings
{

public:

	Settings(const std::string& pathToSettings);
	virtual ~Settings();

	const ApplicationSettings* getApplicationSettings() const { return applicationSettings; }

private:

	ApplicationSettings* extractApplicationSettingsFromFile(const std::string& pathToFile) const;

	std::vector<std::string> splitString(const std::string& string, char delimiter) const;

	ApplicationSettings* applicationSettings;
};
