#include "Application.h"

#include "Settings.h"
#include "Window.h"
#include "Device.h"
#include "ModelLoader.h"
#include "Model.h"
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

	modelLoader = new ModelLoader();
	Model* model = modelLoader->loadModel(applicationSettings);

	render = new Render(device, window, model, applicationSettings);

	delete model;
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