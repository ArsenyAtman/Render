#include "Instance.h"

#include "SystemWindow.h"
#include "Settings.h"

Instance::Instance(SystemWindow* systemWindow, const ApplicationSettings* settings)
{
	instance = createInstance(systemWindow, settings);
}

Instance::~Instance()
{
	vkDestroyInstance(instance, nullptr);
}

VkInstance Instance::createInstance(SystemWindow* systemWindow, const ApplicationSettings* settings) const
{
	if (settings->withValidationLayers && !supportsValidationLayers(settings->validationLayers))
	{
		throw std::runtime_error("Validation layers are not available!");
	}

	VkApplicationInfo applicationInfo = VkApplicationInfo();
	applicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	applicationInfo.pApplicationName = settings->applicationName.data();
	applicationInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	applicationInfo.pEngineName = "None";
	applicationInfo.engineVersion = VK_MAKE_VERSION(0, 0, 0);
	applicationInfo.apiVersion = VK_API_VERSION_1_0;

	VkInstanceCreateInfo instanceCreateInfo = VkInstanceCreateInfo();
	instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	instanceCreateInfo.pApplicationInfo = &applicationInfo;
	uint32_t countOfExtensions = 0;
	const char** requiredExtensions;
	requiredExtensions = systemWindow->getRequiredExtensions(&countOfExtensions);
	instanceCreateInfo.enabledExtensionCount = countOfExtensions;
	instanceCreateInfo.ppEnabledExtensionNames = requiredExtensions;
	if (settings->withValidationLayers)
	{
		instanceCreateInfo.enabledLayerCount = static_cast<uint32_t>(settings->validationLayers.size());
		instanceCreateInfo.ppEnabledLayerNames = settings->validationLayers.data();
	}
	else
	{
		instanceCreateInfo.enabledLayerCount = 0;
	}

	VkInstance instance;
	VkResult result = vkCreateInstance(&instanceCreateInfo, nullptr, &instance);
	if (result != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to create instance!");
	}

	return instance;
}

bool Instance::supportsValidationLayers(const std::vector<const char*>& requestedValidationLayers) const
{
	uint32_t countOfLayers;
	vkEnumerateInstanceLayerProperties(&countOfLayers, nullptr);

	std::vector<VkLayerProperties> availableLayers(countOfLayers);
	vkEnumerateInstanceLayerProperties(&countOfLayers, availableLayers.data());

	for (const char* requestedValidationLayerName : requestedValidationLayers)
	{
		bool layerFound = false;

		for (const VkLayerProperties& availableLayer : availableLayers)
		{
			layerFound = (strcmp(requestedValidationLayerName, availableLayer.layerName) == 0);
			if (layerFound)
			{
				break;
			}
		}

		if (!layerFound)
		{
			return false;
		}
	}

	return true;
}