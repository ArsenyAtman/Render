#pragma once

#include "Tickable.h"

#include <vector>

#include <vulkan/vulkan.h>

class CommandBuffer;
class SwapChainManager;
class GraphicsPipeline;
class SyncsManager;
class Device;
class Window;
class Model;
class Settings;

struct ApplicationSettings;

class Render : public Tickable
{
public:

	Render(Device* device, Window* window, Model* model, const ApplicationSettings* settings);
	virtual ~Render();

	virtual void tick() override;

	uint32_t getCurrentFrame() const { return currentFrame; }

	SwapChainManager* getSwapChain() const { return swapChainManager; }
	GraphicsPipeline* getGraphicsPipeline() const { return graphicsPipeline; }
	CommandBuffer* getCommandBuffer() const { return commandBuffer; }

private:

	SwapChainManager* swapChainManager = nullptr;
	GraphicsPipeline* graphicsPipeline = nullptr;
	CommandBuffer* commandBuffer = nullptr;
	SyncsManager* syncsManager = nullptr;

	Device* device;
	const ApplicationSettings* settings = nullptr;

	uint32_t currentFrame = 0;

	void drawFrame();
};
