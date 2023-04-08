#include "SyncsManager.h"

#include <stdexcept>

SyncsManager::SyncsManager(VkDevice logicalDevice)
{
	VkSemaphoreCreateInfo semaphoreInfo{};
	semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	VkFenceCreateInfo fenceInfo{};
	fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

	VkResult result1 = vkCreateSemaphore(logicalDevice, &semaphoreInfo, nullptr, &imageAvailableSemaphore);
	VkResult result2 = vkCreateSemaphore(logicalDevice, &semaphoreInfo, nullptr, &renderFinishedSemaphore);
	VkResult result3 = vkCreateFence(logicalDevice, &fenceInfo, nullptr, &inFlightFence);
	if (result1 != VK_SUCCESS || result2 != VK_SUCCESS || result3 != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to create semaphores!");
	}

	this->logicalDevice = logicalDevice;
}

SyncsManager::~SyncsManager()
{
	vkDestroySemaphore(logicalDevice, imageAvailableSemaphore, nullptr);
	vkDestroySemaphore(logicalDevice, renderFinishedSemaphore, nullptr);
	vkDestroyFence(logicalDevice, inFlightFence, nullptr);
}