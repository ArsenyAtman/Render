#include "Render.h"

#include <stdexcept>
#include <chrono>
#include <iostream>

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

#include "Settings.h"
#include "Window.h"
#include "Device.h"
#include "Model.h"

Render::Render(Device* device, Window* window, Model* model)
{
	this->device = device;

	commandManager = new CommandManager(device->getLogicalDevice(), device->getQueueIndices());
	vertexBuffer = new VertexBuffer(device->getLogicalDevice(), device->getPhysicalDevice(), model->getMesh());
	uniformBuffer = new UniformBuffer(device->getLogicalDevice(), device->getPhysicalDevice());
	textureImage = new TextureImage(device->getLogicalDevice(), device->getPhysicalDevice(), device->getGraphicsQueue(), commandManager->commandPool, model->getTexture());
	descriptorsManager = new DescriptorsManager(device->getLogicalDevice(), uniformBuffer, textureImage);
	swapChainManager = new SwapChainManager(device->getLogicalDevice(), device->getPhysicalDevice(), device->getGraphicsQueue(), commandManager->commandPool, device->getQueueIndices(), window->getSurface(), window);
	graphicsPipeline = new GraphicsPipeline(device->getLogicalDevice(), swapChainManager->renderPass, descriptorsManager, model->getShaders());
	syncsManager = new SyncsManager(device->getLogicalDevice());
}

Render::~Render()
{
	vkDeviceWaitIdle(device->getLogicalDevice());

	delete syncsManager;
	delete graphicsPipeline;
	delete swapChainManager;
	delete descriptorsManager;
	delete textureImage;
	delete uniformBuffer;
	delete vertexBuffer;
	delete commandManager;
}

void Render::tick()
{
	static std::chrono::steady_clock::time_point startTime = std::chrono::high_resolution_clock::now();

	drawFrame();

	static std::chrono::steady_clock::time_point endTime = std::chrono::high_resolution_clock::now();

	float frameTime = std::chrono::duration<float, std::chrono::milliseconds::period>(endTime - startTime).count();
	//std::cout << "Frame time: " << frameTime << std::endl;
	//std::cout << "FPS: " << 1000.0f / frameTime << std::endl;
}

void Render::drawFrame()
{
	vkWaitForFences(device->getLogicalDevice(), 1, &syncsManager->inFlightFence, VK_TRUE, UINT64_MAX);
	vkResetFences(device->getLogicalDevice(), 1, &syncsManager->inFlightFence);

	uint32_t imageIndex;
	vkAcquireNextImageKHR(device->getLogicalDevice(), swapChainManager->swapChain, UINT64_MAX, syncsManager->imageAvailableSemaphore, VK_NULL_HANDLE, &imageIndex);

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

	VkResult result = vkQueueSubmit(device->getGraphicsQueue(), 1, &submitInfo, syncsManager->inFlightFence);
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

	vkQueuePresentKHR(device->getPresentQueue(), &presentInfo);
}
