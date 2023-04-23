#include "SyncsManager.h"

#include <stdexcept>

#include "Render.h"
#include "Device.h"
#include "Settings.h"

SyncsManager::SyncsManager(Render* render, Device* device, const ApplicationSettings* settings) : RenderModule(render, device, settings)
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
		VkResult imageAvailableSemaphoreCreationResult = vkCreateSemaphore(getDevice()->getLogicalDevice(), &semaphoreInfo, nullptr, &imageAvailableSemaphores[i]);
		VkResult renderFinishedSemaphoreCreationResult = vkCreateSemaphore(getDevice()->getLogicalDevice(), &semaphoreInfo, nullptr, &renderFinishedSemaphores[i]);
		VkResult flightFenceCreationResult = vkCreateFence(getDevice()->getLogicalDevice(), &fenceInfo, nullptr, &inFlightFences[i]);
		if (imageAvailableSemaphoreCreationResult != VK_SUCCESS || renderFinishedSemaphoreCreationResult != VK_SUCCESS || flightFenceCreationResult != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create semaphores!");
		}
	}
}

SyncsManager::~SyncsManager()
{
	for (size_t i = 0; i < imageAvailableSemaphores.size(); ++i)
	{
		vkDestroySemaphore(getDevice()->getLogicalDevice(), imageAvailableSemaphores[i], nullptr);
	}

	for (size_t i = 0; i < renderFinishedSemaphores.size(); ++i)
	{
		vkDestroySemaphore(getDevice()->getLogicalDevice(), renderFinishedSemaphores[i], nullptr);
	}

	for (size_t i = 0; i < inFlightFences.size(); ++i)
	{
		vkDestroyFence(getDevice()->getLogicalDevice(), inFlightFences[i], nullptr);
	}
}

VkSemaphore SyncsManager::getImageAvailableSemaphoreForCurrentFrame() const
{
	return imageAvailableSemaphores[getRender()->getCurrentFrame()];
}

VkSemaphore SyncsManager::getRenderFinishedSemaphoreForCurrentFrame() const
{
	return renderFinishedSemaphores[getRender()->getCurrentFrame()];
}

VkFence SyncsManager::getInFlightFenceForCurrentFrame() const
{
	return inFlightFences[getRender()->getCurrentFrame()];
}