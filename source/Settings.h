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

	std::string pathToCompiler = "";
	std::string temporaryBatFile = "";
	std::vector<std::string> shadersToCompile = {};
	std::vector<std::string> compileShadersTo = {};

	bool withValidationLayers = false;
	std::vector<const char*> validationLayers = {};
	std::vector<const char*> deviceExtensions = {};
	uint32_t maxFramesInFlight = 0;

	std::vector<float> viewPoint = {};
	std::vector<float> viewTarget = {};
	float fov = 0.0f;
	float perspectiveNear = 0.0f;
	float perspectiveFar = 0.0f;

	std::string meshPath;
	std::string texturePath;
	std::vector<std::string> shaderPaths;
	std::vector<float> rotation = {};
	float rotationSpeed = 0.0f;
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
