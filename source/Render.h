#pragma once

#include "Tickable.h"

#include <vector>

#include <vulkan/vulkan.h>

class SwapChainManager;
class GraphicsPipeline;
class CommandManager;
class SyncsManager;

class Settings;
class Window;
class Device;
class Model;
class Buffer;

struct ApplicationSettings;

class Render : public Tickable
{
public:

	Render(Device* device, Window* window, Model* model, const ApplicationSettings* settings);
	virtual ~Render();

	virtual void tick() override;

	uint32_t getCurrentFrame() const { return currentFrame; }

	const std::vector<Buffer*>& getBuffers() const { return buffers; }
	Buffer* getIndexBuffer() const { return indexBuffer; }

	SwapChainManager* getSwapChain() const { return swapChainManager; }
	GraphicsPipeline* getGraphicsPipeline() const { return graphicsPipeline; }
	CommandManager* getCommandBuffer() const { return commandManager; }

private:

	std::vector<Buffer*> buffers;
	Buffer* indexBuffer = nullptr;

	SwapChainManager* swapChainManager = nullptr;
	GraphicsPipeline* graphicsPipeline = nullptr;
	CommandManager* commandManager = nullptr;
	SyncsManager* syncsManager = nullptr;

	Device* device;
	const ApplicationSettings* settings = nullptr;

	uint32_t currentFrame = 0;

	void drawFrame();
};
