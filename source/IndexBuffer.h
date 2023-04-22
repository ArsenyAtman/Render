#pragma once

#include "Buffer.h"

#include <vulkan/vulkan.h>

class Mesh;

class IndexBuffer : public Buffer
{
public:

	IndexBuffer(Render* render, Device* device, const ApplicationSettings* settings, const Mesh* mesh);

	virtual void bindToCommandBuffer(VkCommandBuffer commandBuffer) const override;
};
