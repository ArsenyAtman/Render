#include "Render.h"

#include <stdexcept>

#include "WindowManager.h"
#include "SurfaceManager.h"
#include "InstanceManager.h"
#include "PhysicalDeviceManager.h"
#include "LogicalDeviceManager.h"
#include "SwapChainManager.h"
#include "GraphicsPipeline.h"
#include "VertexBuffer.h"
#include "CommandManager.h"
#include "SyncsManager.h"

#include "Vertex.h"

Render::Render()
{
	const std::vector<Vertex> vertices =
	{
		{{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
		{{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
		{{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
		{{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}},
		{{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
		{{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}
	};

	windowManager = new WindowManager(800, 600, "Vulkan Render");
	instanceManager = new InstanceManager(windowManager, enableValidationLayers, validationLayers);
	surfaceManager = new SurfaceManager(instanceManager->getInstance(), windowManager->getWindow());
	physicalDeviceManager = new PhysicalDeviceManager(instanceManager->getInstance(), surfaceManager->getSurface(), deviceExtensions);
	logicalDeviceManager = new LogicalDeviceManager(physicalDeviceManager->getDevice(), physicalDeviceManager->getQueueFamilyIndices(), enableValidationLayers, validationLayers, deviceExtensions);
	vertexBuffer = new VertexBuffer(logicalDeviceManager->getDevice(), physicalDeviceManager->getDevice(), vertices);
	swapChainManager = new SwapChainManager(physicalDeviceManager->getDevice(), physicalDeviceManager->getQueueFamilyIndices(), surfaceManager->getSurface(), logicalDeviceManager->getDevice(), windowManager);
	graphicsPipeline = new GraphicsPipeline(logicalDeviceManager->getDevice(), swapChainManager->renderPass);
	commandManager = new CommandManager(logicalDeviceManager->getDevice(), physicalDeviceManager->getQueueFamilyIndices());
	syncsManager = new SyncsManager(logicalDeviceManager->getDevice());

	while (!windowManager->isClosed())
	{
		windowManager->tick();
		drawFrame();
	}

	vkDeviceWaitIdle(logicalDeviceManager->getDevice());

	delete syncsManager;
	delete commandManager;
	delete graphicsPipeline;
	delete swapChainManager;
	delete vertexBuffer;
	delete logicalDeviceManager;
	delete physicalDeviceManager;
	delete surfaceManager;
	delete instanceManager;
	delete windowManager;
}

void Render::drawFrame()
{
	vkWaitForFences(logicalDeviceManager->getDevice(), 1, &syncsManager->inFlightFence, VK_TRUE, UINT64_MAX);
	vkResetFences(logicalDeviceManager->getDevice(), 1, &syncsManager->inFlightFence);

	uint32_t imageIndex;
	vkAcquireNextImageKHR(logicalDeviceManager->getDevice(), swapChainManager->swapChain, UINT64_MAX, syncsManager->imageAvailableSemaphore, VK_NULL_HANDLE, &imageIndex);

	commandManager->recordCommandBuffer(imageIndex, swapChainManager, graphicsPipeline->graphicsPipeline, vertexBuffer);

	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

	VkSemaphore waitSemaphores[] = { syncsManager->imageAvailableSemaphore };
	VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = waitSemaphores;
	submitInfo.pWaitDstStageMask = waitStages;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandManager->commandBuffer;

	VkSemaphore signalSemaphores[] = { syncsManager->renderFinishedSemaphore };
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = signalSemaphores;

	VkResult result = vkQueueSubmit(logicalDeviceManager->getGraphicsQueue(), 1, &submitInfo, syncsManager->inFlightFence);
	if (result != VK_SUCCESS)
	{
		throw std::runtime_error("failed to submit draw command buffer!");
	}

	VkPresentInfoKHR presentInfo{};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = signalSemaphores;
	VkSwapchainKHR swapChains[] = { swapChainManager->swapChain };
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = swapChains;
	presentInfo.pImageIndices = &imageIndex;
	presentInfo.pResults = nullptr; // Optional

	vkQueuePresentKHR(logicalDeviceManager->getPresentQueue(), &presentInfo);
}
