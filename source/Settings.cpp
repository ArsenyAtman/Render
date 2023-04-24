#include "Settings.h"

#include <stdexcept>

Settings::Settings(const std::string& pathToSettings)
{
	applicationSettings = extractApplicationSettingsFromFile(pathToSettings);
	if (applicationSettings == nullptr)
	{
		throw std::runtime_error("Failed to parse the settings ini!");
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

	std::string& applicationName = ini["application"]["applicationName"];

	std::string& windowWidth = ini["window"]["width"];
	std::string& windowHeight = ini["window"]["height"];
	std::string& windowName = ini["window"]["name"];

	std::string pathToCompiler = ini["shaderCompiler"]["pathToCompiler"];
	std::string temporaryBatFile = ini["shaderCompiler"]["temporaryBatFile"];
	std::vector<std::string> shadersToCompile = splitString(ini["shaderCompiler"]["shadersToCompile"], ',');
	std::vector<std::string> compileShadersTo = splitString(ini["shaderCompiler"]["compileShadersTo"], ',');

	std::string& withValidationLayers = ini["render"]["withValidationLayers"];
	std::vector<std::string> validationLayers = splitString(ini["render"]["validationLayers"], ',');
	std::vector<std::string> deviceExtensions = splitString(ini["render"]["deviceExtensions"], ',');
	std::string& maxFramesInFlight = ini["render"]["maxFramesInFlight"];

	std::vector<std::string> viewPoint = splitString(ini["view"]["viewPoint"], ',');
	std::vector<std::string> viewTarget = splitString(ini["view"]["viewTarget"], ',');
	std::string& fov = ini["view"]["fov"];
	std::string& perspectiveNear = ini["view"]["perspectiveNear"];
	std::string& perspectiveFar = ini["view"]["perspectiveFar"];

	std::string& modelPath = ini["model"]["pathToModel"];
	std::string& texturePath = ini["model"]["pathToTexture"];
	std::vector<std::string> shaderPaths = splitString(ini["model"]["pathsToShaders"], ',');
	std::vector<std::string> rotation = splitString(ini["model"]["rotation"], ',');
	std::string& rotationSpeed = ini["model"]["rotationSpeed"];

	ApplicationSettings* applicationSettings = new ApplicationSettings();

	applicationSettings->applicationName = applicationName;

	applicationSettings->windowWidth = static_cast<uint32_t>(std::stoul(windowWidth));
	applicationSettings->windowHeight = static_cast<uint32_t>(std::stoul(windowHeight));
	applicationSettings->windowName = windowName;

	applicationSettings->pathToCompiler = pathToCompiler;
	applicationSettings->temporaryBatFile = temporaryBatFile;
	applicationSettings->shadersToCompile = shadersToCompile;
	applicationSettings->compileShadersTo = compileShadersTo;

	applicationSettings->withValidationLayers = static_cast<bool>(std::stoul(withValidationLayers));
	for (const std::string& validationLayer : validationLayers)
	{
		char* string = new char[validationLayer.length() + 1];
		validationLayer.copy(string, validationLayer.length());
		string[validationLayer.length()] = '\0';
		applicationSettings->validationLayers.push_back(string);
	}
	for (const std::string& deviceExtension : deviceExtensions)
	{
		char* string = new char[deviceExtension.length() + 1];
		deviceExtension.copy(string, deviceExtension.length());
		string[deviceExtension.length()] = '\0';
		applicationSettings->deviceExtensions.push_back(string);
	}
	applicationSettings->maxFramesInFlight = static_cast<uint32_t>(std::stoul(maxFramesInFlight));

	for (const std::string& viewPointDimension : viewPoint)
	{
		applicationSettings->viewPoint.push_back(static_cast<float>(std::stoul(viewPointDimension)));
	}
	for (const std::string& viewTargetDimension : viewTarget)
	{
		applicationSettings->viewTarget.push_back(static_cast<float>(std::stoul(viewTargetDimension)));
	}
	applicationSettings->fov = static_cast<float>(std::stoul(fov));
	applicationSettings->perspectiveNear = static_cast<float>(std::stoul(perspectiveNear));
	applicationSettings->perspectiveFar = static_cast<float>(std::stoul(perspectiveFar));

	applicationSettings->meshPath = modelPath;
	applicationSettings->texturePath = texturePath;
	applicationSettings->shaderPaths = shaderPaths;
	for (const std::string& rotationDimension : rotation)
	{
		applicationSettings->rotation.push_back(static_cast<float>(std::stoul(rotationDimension)));
	}
	applicationSettings->rotationSpeed = static_cast<float>(std::stoul(rotationSpeed));

	return applicationSettings;
}

std::vector<std::string> Settings::splitString(const std::string& string, char delimiter) const
{
	std::vector<std::string> result;
	std::stringstream stringStream(string);
	std::string item;

	while (getline(stringStream, item, delimiter))
	{
		result.push_back(item);
	}

	return result;
}
