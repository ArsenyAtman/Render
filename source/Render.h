#pragma once

#include <vector>

#include <vulkan/vulkan.h>

class IndexBuffer;
class VertexBuffer;
class UniformBuffer;
class SwapChainManager;
class GraphicsPipeline;
class CommandManager;
class SyncsManager;
class TextureImage;
class DescriptorsManager;

class Settings;
class Window;
class Device;
class Model;

struct ApplicationSettings;

class Render
{
public:

	Render(Device* device, Window* window, Model* model, const ApplicationSettings* settings);
	virtual ~Render();

	void tick();

	VertexBuffer* getVertexBuffer() const { return vertexBuffer; }
	IndexBuffer* getIndexBuffer() const { return indexBuffer; }
	//UniformBuffer* getUniformBuffer() const { return uniformBuffer; }
	//TextureImage* getTextureImage() const { return textureImage; }
	DescriptorsManager* getDescriptorsManager() const { return descriptorsManager; }
	SwapChainManager* getSwapChain() const { return swapChainManager; }
	GraphicsPipeline* getGraphicsPipeline() const { return graphicsPipeline; }
	CommandManager* getCommandBuffer() const { return commandManager; }

private:

	VertexBuffer* vertexBuffer = nullptr;
	IndexBuffer* indexBuffer = nullptr;
	UniformBuffer* uniformBuffer = nullptr;
	TextureImage* textureImage = nullptr;
	DescriptorsManager* descriptorsManager = nullptr;
	SwapChainManager* swapChainManager = nullptr;
	GraphicsPipeline* graphicsPipeline = nullptr;
	CommandManager* commandManager = nullptr;
	SyncsManager* syncsManager = nullptr;

	Device* device;
	const ApplicationSettings* settings = nullptr;

	uint32_t currentFrame = 0;

	void drawFrame();
};
