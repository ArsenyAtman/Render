#include "Render.h"

#include <stdexcept>
#include <chrono>
#include <iostream>

#include "SwapChainManager.h"
#include "GraphicsPipeline.h"
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
#include "Buffer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

Render::Render(Device* device, Window* window, Model* model, const ApplicationSettings* settings)
{
	this->device = device;
	this->settings = settings;

	commandManager = new CommandManager(this, device, settings);
	buffers.push_back(new VertexBuffer(this, device, settings, model->getMesh()));
	indexBuffer = new IndexBuffer(this, device, settings, model->getMesh());
	buffers.push_back(indexBuffer);

	uniformBuffer = new UniformBuffer(device->getLogicalDevice(), device->getPhysicalDevice(), settings);
	textureImage = new TextureImage(device->getLogicalDevice(), device->getPhysicalDevice(), device->getGraphicsQueue(), commandManager->commandPool, model->getTexture());
	descriptorsManager = new DescriptorsManager(this, device, settings);
	swapChainManager = new SwapChainManager(device->getLogicalDevice(), device->getPhysicalDevice(), device->getGraphicsQueue(), commandManager->commandPool, device->getQueueIndices(), window->getSurface(), window);
	graphicsPipeline = new GraphicsPipeline(this, device, settings, model->getShaders());
	syncsManager = new SyncsManager(device->getLogicalDevice(), settings);
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
	for (Buffer* buffer : buffers)
	{
		delete buffer;
	}
	delete commandManager;
}

void Render::tick()
{
	drawFrame();
}

void Render::drawFrame()
{
	vkWaitForFences(device->getLogicalDevice(), 1, &syncsManager->inFlightFences[currentFrame], VK_TRUE, UINT64_MAX);
	vkResetFences(device->getLogicalDevice(), 1, &syncsManager->inFlightFences[currentFrame]);

	uint32_t imageIndex;
	vkAcquireNextImageKHR(device->getLogicalDevice(), swapChainManager->swapChain, UINT64_MAX, syncsManager->imageAvailableSemaphores[currentFrame], VK_NULL_HANDLE, &imageIndex);

	uniformBuffer->update(currentFrame, swapChainManager->swapChainExtent);

	commandManager->recordCommandBuffer(currentFrame, imageIndex);

	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

	VkSemaphore waitSemaphores[] = { syncsManager->imageAvailableSemaphores[currentFrame]};
	VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = waitSemaphores;
	submitInfo.pWaitDstStageMask = waitStages;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandManager->commandBuffers[currentFrame];

	VkSemaphore signalSemaphores[] = { syncsManager->renderFinishedSemaphores[currentFrame]};
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = signalSemaphores;

	VkResult result = vkQueueSubmit(device->getGraphicsQueue(), 1, &submitInfo, syncsManager->inFlightFences[currentFrame]);
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

	currentFrame = (currentFrame + 1) % settings->maxFramesInFlight;
}
