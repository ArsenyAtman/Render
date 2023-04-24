#include "Render.h"

#include <stdexcept>
#include <chrono>
#include <iostream>

#include "SwapChain.h"
#include "GraphicsPipeline.h"
#include "CommandBuffer.h"
#include "SyncsManager.h"
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

	commandBuffer = new CommandBuffer(this, device, settings, model);
	swapChain = new SwapChain(this, device, settings, window);
	graphicsPipeline = new GraphicsPipeline(this, device, settings, model);
	syncsManager = new SyncsManager(this, device, settings);
}

Render::~Render()
{
	vkDeviceWaitIdle(device->getLogicalDevice());

	delete syncsManager;
	delete graphicsPipeline;
	delete swapChain;
	delete commandBuffer;
}

void Render::tick()
{
	graphicsPipeline->tick();

	drawFrame();

	currentFrame = (currentFrame + 1) % settings->maxFramesInFlight;
}

void Render::drawFrame()
{
	vkWaitForFences(device->getLogicalDevice(), 1, &syncsManager->getInFlightFenceForCurrentFrame(), VK_TRUE, UINT64_MAX);
	vkResetFences(device->getLogicalDevice(), 1, &syncsManager->getInFlightFenceForCurrentFrame());

	uint32_t imageIndex;
	vkAcquireNextImageKHR(device->getLogicalDevice(), swapChain->getSwapChain(), UINT64_MAX, syncsManager->getImageAvailableSemaphoreForCurrentFrame(), VK_NULL_HANDLE, &imageIndex);

	commandBuffer->recordCommandBuffer(imageIndex);

	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

	VkSemaphore waitSemaphores[] = { syncsManager->getImageAvailableSemaphoreForCurrentFrame()};
	VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = waitSemaphores;
	submitInfo.pWaitDstStageMask = waitStages;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffer->getCommandBufferForCurrentFrame();

	VkSemaphore signalSemaphores[] = { syncsManager->getRenderFinishedSemaphoreForCurrentFrame()};
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = signalSemaphores;

	VkResult result = vkQueueSubmit(device->getGraphicsQueue(), 1, &submitInfo, syncsManager->getInFlightFenceForCurrentFrame());
	if (result != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to submit the draw command buffer!");
	}

	VkPresentInfoKHR presentInfo{};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = signalSemaphores;
	VkSwapchainKHR swapChains[] = { swapChain->getSwapChain()};
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = swapChains;
	presentInfo.pImageIndices = &imageIndex;

	vkQueuePresentKHR(device->getPresentQueue(), &presentInfo);
}
