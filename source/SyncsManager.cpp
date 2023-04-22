#include "SyncsManager.h"

#include <stdexcept>

#include "Settings.h"

SyncsManager::SyncsManager(VkDevice logicalDevice, const ApplicationSettings* settings)
{
	imageAvailableSemaphores.resize(settings->maxFramesInFlight);
	renderFinishedSemaphores.resize(settings->maxFramesInFlight);
	inFlightFences.resize(settings->maxFramesInFlight);

	VkSemaphoreCreateInfo semaphoreInfo{};
	semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	VkFenceCreateInfo fenceInfo{};
	fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

	for (size_t i = 0; i < settings->maxFramesInFlight; ++i)
	{
		VkResult result1 = vkCreateSemaphore(logicalDevice, &semaphoreInfo, nullptr, &imageAvailableSemaphores[i]);
		VkResult result2 = vkCreateSemaphore(logicalDevice, &semaphoreInfo, nullptr, &renderFinishedSemaphores[i]);
		VkResult result3 = vkCreateFence(logicalDevice, &fenceInfo, nullptr, &inFlightFences[i]);
		if (result1 != VK_SUCCESS || result2 != VK_SUCCESS || result3 != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create semaphores!");
		}
	}

	this->logicalDevice = logicalDevice;
}

SyncsManager::~SyncsManager()
{
	for (size_t i = 0; i < imageAvailableSemaphores.size(); ++i)
	{
		vkDestroySemaphore(logicalDevice, imageAvailableSemaphores[i], nullptr);
	}

	for (size_t i = 0; i < renderFinishedSemaphores.size(); ++i)
	{
		vkDestroySemaphore(logicalDevice, renderFinishedSemaphores[i], nullptr);
	}

	for (size_t i = 0; i < inFlightFences.size(); ++i)
	{
		vkDestroyFence(logicalDevice, inFlightFences[i], nullptr);
	}
}