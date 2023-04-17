#pragma once

#include <vector>

#include <vulkan/vulkan.h>

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

class Render
{
public:

	Render(Device* device, Window* window, Model* model);
	virtual ~Render();

	void tick();

private:

	VertexBuffer* vertexBuffer = nullptr;
	UniformBuffer* uniformBuffer = nullptr;
	TextureImage* textureImage = nullptr;
	DescriptorsManager* descriptorsManager = nullptr;
	SwapChainManager* swapChainManager = nullptr;
	GraphicsPipeline* graphicsPipeline = nullptr;
	CommandManager* commandManager = nullptr;
	SyncsManager* syncsManager = nullptr;

	Device* device;

	void drawFrame();
};
