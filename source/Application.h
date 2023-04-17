#pragma once

class Settings;
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
	Window* window = nullptr;
	Device* device = nullptr;
	ModelLoader* modelLoader = nullptr;
	Render* render = nullptr;
};
