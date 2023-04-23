#include "DepthBuffer.h"

#include "Render.h"
#include "Device.h"
#include "Settings.h"
#include "SwapChain.h"
#include "Helpers.h"

DepthBuffer::DepthBuffer(Render* render, Device* device, const ApplicationSettings* settings, VkExtent2D swapChainExtent) : RenderModule(render, device, settings)
{
	VkFormat depthFormat = Helpers::findDepthFormat(getDevice()->getPhysicalDevice());

	Helpers::createImage(getDevice()->getLogicalDevice(), getDevice()->getPhysicalDevice(), swapChainExtent.width, swapChainExtent.height, depthFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, depthImage, depthImageMemory);
	depthImageView = Helpers::createImageView(getDevice()->getLogicalDevice(), depthImage, depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT);

	Helpers::transitionImageLayout(device, depthImage, depthFormat, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
}

DepthBuffer::~DepthBuffer()
{
	vkDestroyImageView(getDevice()->getLogicalDevice(), depthImageView, nullptr);
	vkDestroyImage(getDevice()->getLogicalDevice(), depthImage, nullptr);
	vkFreeMemory(getDevice()->getLogicalDevice(), depthImageMemory, nullptr);
}
