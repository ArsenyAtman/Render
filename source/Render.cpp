#include "Render.h"

#include <stdexcept>
#include <chrono>
#include <iostream>

#include "WindowManager.h"
#include "SurfaceManager.h"
#include "InstanceManager.h"
#include "PhysicalDeviceManager.h"
#include "LogicalDeviceManager.h"
#include "SwapChainManager.h"
#include "GraphicsPipeline.h"
#include "VertexBuffer.h"
#include "UniformBuffer.h"
#include "CommandManager.h"
#include "SyncsManager.h"
#include "TextureImage.h"
#include "DescriptorsManager.h"
#include "ModelLoader.h"

#include "Vertex.h"

Render::Render()
{
	modelLoader = new ModelLoader();

	windowManager = new WindowManager(1280, 720, "Vulkan Render");
	instanceManager = new InstanceManager(windowManager, enableValidationLayers, validationLayers);
	surfaceManager = new SurfaceManager(instanceManager->getInstance(), windowManager->getWindow());
	physicalDeviceManager = new PhysicalDeviceManager(instanceManager->getInstance(), surfaceManager->getSurface(), deviceExtensions);
	logicalDeviceManager = new LogicalDeviceManager(physicalDeviceManager->getDevice(), physicalDeviceManager->getQueueFamilyIndices(), enableValidationLayers, validationLayers, deviceExtensions);
	commandManager = new CommandManager(logicalDeviceManager->getDevice(), physicalDeviceManager->getQueueFamilyIndices());
	vertexBuffer = new VertexBuffer(logicalDeviceManager->getDevice(), physicalDeviceManager->getDevice(), modelLoader->vertices);
	uniformBuffer = new UniformBuffer(logicalDeviceManager->getDevice(), physicalDeviceManager->getDevice());
	textureImage = new TextureImage(logicalDeviceManager->getDevice(), physicalDeviceManager->getDevice(), logicalDeviceManager->getGraphicsQueue(), commandManager->commandPool);
	descriptorsManager = new DescriptorsManager(logicalDeviceManager->getDevice(), uniformBuffer, textureImage);
	swapChainManager = new SwapChainManager(logicalDeviceManager->getDevice(), physicalDeviceManager->getDevice(), logicalDeviceManager->getGraphicsQueue(), commandManager->commandPool, physicalDeviceManager->getQueueFamilyIndices(), surfaceManager->getSurface(), windowManager);
	graphicsPipeline = new GraphicsPipeline(logicalDeviceManager->getDevice(), swapChainManager->renderPass, descriptorsManager);
	syncsManager = new SyncsManager(logicalDeviceManager->getDevice());

	while (!windowManager->isClosed())
	{
		static std::chrono::steady_clock::time_point startTime = std::chrono::high_resolution_clock::now();

		windowManager->tick();
		drawFrame();

		static std::chrono::steady_clock::time_point endTime = std::chrono::high_resolution_clock::now();

		float frameTime = std::chrono::duration<float, std::chrono::milliseconds::period>(endTime - startTime).count();
		//std::cout << "Frame time: " << frameTime << std::endl;
		//std::cout << "FPS: " << 1000.0f / frameTime << std::endl;
	}

	vkDeviceWaitIdle(logicalDeviceManager->getDevice());

	delete syncsManager;
	delete graphicsPipeline;
	delete swapChainManager;
	delete descriptorsManager;
	delete textureImage;
	delete uniformBuffer;
	delete vertexBuffer;
	delete commandManager;
	delete logicalDeviceManager;
	delete physicalDeviceManager;
	delete surfaceManager;
	delete instanceManager;
	delete windowManager;

	delete modelLoader;
}

void Render::drawFrame()
{
	vkWaitForFences(logicalDeviceManager->getDevice(), 1, &syncsManager->inFlightFence, VK_TRUE, UINT64_MAX);
	vkResetFences(logicalDeviceManager->getDevice(), 1, &syncsManager->inFlightFence);

	uint32_t imageIndex;
	vkAcquireNextImageKHR(logicalDeviceManager->getDevice(), swapChainManager->swapChain, UINT64_MAX, syncsManager->imageAvailableSemaphore, VK_NULL_HANDLE, &imageIndex);

	uniformBuffer->update(0, swapChainManager->swapChainExtent);

	commandManager->recordCommandBuffer(imageIndex, swapChainManager, graphicsPipeline, vertexBuffer, descriptorsManager);

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

	vkQueuePresentKHR(logicalDeviceManager->getPresentQueue(), &presentInfo);
}
