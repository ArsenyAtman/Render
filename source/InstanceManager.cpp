#include "InstanceManager.h"

#include <stdexcept>

#include "WindowManager.h"

InstanceManager::InstanceManager(WindowManager* windowManager, bool withValidationLayers, const std::vector<const char*>& validationLayers)
{
	if (withValidationLayers && !checkValidationLayersSupport(validationLayers))
	{
		throw std::runtime_error("Validation layers are not available!");
	}

	VkApplicationInfo applicationInfo = VkApplicationInfo();
	applicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	applicationInfo.pApplicationName = "Vulkan Render";
	applicationInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	applicationInfo.pEngineName = "None";
	applicationInfo.engineVersion = VK_MAKE_VERSION(0, 0, 0);
	applicationInfo.apiVersion = VK_API_VERSION_1_0;

	VkInstanceCreateInfo instanceCreateInfo = VkInstanceCreateInfo();
	instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	instanceCreateInfo.pApplicationInfo = &applicationInfo;
	uint32_t countOfExtensions = 0;
	const char** extensions;
	extensions = windowManager->getRequiredExtensions(&countOfExtensions);
	instanceCreateInfo.enabledExtensionCount = countOfExtensions;
	instanceCreateInfo.ppEnabledExtensionNames = extensions;
	if (withValidationLayers)
	{
		instanceCreateInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
		instanceCreateInfo.ppEnabledLayerNames = validationLayers.data();
	}
	else
	{
		instanceCreateInfo.enabledLayerCount = 0;
	}

	VkResult result = vkCreateInstance(&instanceCreateInfo, nullptr, &instance);
	if (result != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to create vulkan instance!");
	}
}

InstanceManager::~InstanceManager()
{
	vkDestroyInstance(instance, nullptr);
}

bool InstanceManager::checkValidationLayersSupport(const std::vector<const char*>& requestedValidationLayers)
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