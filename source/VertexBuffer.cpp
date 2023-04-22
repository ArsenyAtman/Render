#include "VertexBuffer.h"

#include <vector>
#include <stdexcept>

#include "Mesh.h"

VertexBuffer::VertexBuffer(Render* render, Device* device, const ApplicationSettings* settings, const Mesh* mesh) : MeshBuffer(render, device, settings)
{
	constructBuffer(mesh->getVertices().data(), sizeof(mesh->getVertices()[0]), mesh->getVertices().size(), VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);
}

void VertexBuffer::bindBuffer(VkCommandBuffer commandBuffer)
{
	VkBuffer vertexBuffers[] = { getBuffer() };
	VkDeviceSize offsets[] = { 0 };
	vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);
}