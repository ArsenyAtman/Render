#include "Application.h"

#include "Settings.h"
#include "Window.h"
#include "Device.h"
#include "Render.h"

Application::Application()
{
	initSystems();

	while (!window->shouldExit())
	{
		tick();
	}

	deinitSystems();
}

void Application::initSystems()
{
	settings = new Settings("settings/settings.ini");
	const ApplicationSettings* applicationSettings = settings->getApplicationSettings();

	window = new Window(applicationSettings);
	device = new Device(window, applicationSettings);

	render = new Render(device, window);
}

void Application::tick()
{
	window->tick();
	render->tick();
}

void Application::deinitSystems()
{
	delete render;
	delete device;
	delete window;
	delete settings;
}