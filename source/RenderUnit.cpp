#include "RenderUnit.h"

#include "Device.h"

RenderUnit::RenderUnit(Device* device)
{
	this->device = device;
}

RenderUnit::~RenderUnit()
{
	device = nullptr;
}