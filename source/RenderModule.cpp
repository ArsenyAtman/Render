#include "RenderModule.h"

#include "Render.h"
#include "Device.h"
#include "Settings.h"

RenderModule::RenderModule(Render* render, Device* device, const ApplicationSettings* settings)
{
	this->render = render;
	this->device = device;
	this->settings = settings;
}

RenderModule::~RenderModule()
{
	// ...
}
