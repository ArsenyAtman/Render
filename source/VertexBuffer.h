#pragma once

#include "MeshBuffer.h"

#include <vector>

#include <vulkan/vulkan.h>

class Mesh;

class VertexBuffer : public MeshBuffer
{

public:

	VertexBuffer(Render* render, Device* device, const ApplicationSettings* settings, const Mesh* mesh);

	virtual void bindBuffer(VkCommandBuffer commandBuffer) override;
};

