#pragma once

class Settings;
class ShaderCompiler;
class Window;
class Device;
class ModelLoader;
class Render;

class Application
{
public:

	Application();

private:

	void initSystems();
	void tick();
	void deinitSystems();

	Settings* settings = nullptr;
	ShaderCompiler* shaderCompiler = nullptr;
	Window* window = nullptr;
	Device* device = nullptr;
	ModelLoader* modelLoader = nullptr;
	Render* render = nullptr;
};
