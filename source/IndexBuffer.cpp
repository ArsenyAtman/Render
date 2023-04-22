#include "IndexBuffer.h"

#include "Helpers.h"
#include "Mesh.h"

IndexBuffer::IndexBuffer(Render* render, Device* device, const ApplicationSettings* settings, const Mesh* mesh) : Buffer(render, device, settings)
{
	constructBuffer(mesh->getIndices().data(), sizeof(mesh->getIndices()[0]), mesh->getIndices().size(), VK_BUFFER_USAGE_INDEX_BUFFER_BIT);
}

void IndexBuffer::bindToCommandBuffer(VkCommandBuffer commandBuffer) const
{
	vkCmdBindIndexBuffer(commandBuffer, getBuffer(), 0, VK_INDEX_TYPE_UINT32);
}
