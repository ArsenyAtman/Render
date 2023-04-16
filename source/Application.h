#pragma once

class Settings;
class Window;
class Device;
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
	Render* render = nullptr;

};
