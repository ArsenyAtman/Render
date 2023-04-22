#pragma once

#include "MeshBuffer.h"

#include <vulkan/vulkan.h>

class Mesh;

class IndexBuffer : public MeshBuffer
{
public:

	IndexBuffer(Render* render, Device* device, const ApplicationSettings* settings, const Mesh* mesh);

	virtual void bindBuffer(VkCommandBuffer commandBuffer) override;
};
