#pragma once

#include "Tickable.h"

#include <vector>

#include <vulkan/vulkan.h>

class CommandBuffer;
class SwapChain;
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

	SwapChain* getSwapChain() const { return swapChain; }
	GraphicsPipeline* getGraphicsPipeline() const { return graphicsPipeline; }
	CommandBuffer* getCommandBuffer() const { return commandBuffer; }

private:

	SwapChain* swapChain = nullptr;
	GraphicsPipeline* graphicsPipeline = nullptr;
	CommandBuffer* commandBuffer = nullptr;
	SyncsManager* syncsManager = nullptr;

	Device* device;
	const ApplicationSettings* settings = nullptr;

	uint32_t currentFrame = 0;

	void drawFrame();
};
