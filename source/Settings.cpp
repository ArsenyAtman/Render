#include "Settings.h"

#include <stdexcept>

Settings::Settings(const std::string& pathToSettings)
{
	applicationSettings = extractApplicationSettingsFromFile(pathToSettings);
	if (applicationSettings == nullptr)
	{
		throw std::runtime_error("Failed to parse settings ini!");
	}
}

Settings::~Settings()
{
	delete applicationSettings;
}

ApplicationSettings* Settings::extractApplicationSettingsFromFile(const std::string& pathToFile) const
{
	mINI::INIFile file(pathToFile);
	mINI::INIStructure ini;
	file.read(ini);

	std::string& windowWidth = ini["window"]["width"];
	std::string& windowHeight = ini["window"]["height"];
	std::string& windowName = ini["window"]["name"];

	ApplicationSettings* applicationSettings = new ApplicationSettings();
	applicationSettings->windowWidth = static_cast<uint32_t>(std::stoul(windowWidth));
	applicationSettings->windowHeight = static_cast<uint32_t>(std::stoul(windowHeight));
	applicationSettings->windowName = windowName;

	return applicationSettings;
}