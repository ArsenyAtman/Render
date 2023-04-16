#pragma once

#include <vulkan/vulkan.h>

class Device;

class RenderUnit
{
public:

	RenderUnit(Device* device);
	virtual ~RenderUnit();

	Device* getDevice() const { return device; }

private:

	Device* device = nullptr;
};

