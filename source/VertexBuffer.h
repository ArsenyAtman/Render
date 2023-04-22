#pragma once

#include "Buffer.h"

#include <vector>

#include <vulkan/vulkan.h>

class Mesh;

class VertexBuffer : public Buffer
{

public:

	VertexBuffer(Render* render, Device* device, const ApplicationSettings* settings, const Mesh* mesh);

	virtual void bindToCommandBuffer(VkCommandBuffer commandBuffer) const override;
};

