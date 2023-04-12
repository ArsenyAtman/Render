#include <iostream>
#include <stdexcept>
#include <cstdlib>

#include "Render.h"

/**
 * TODO: Handle VK_ERROR_INCOMPATIBLE_DRIVER.
 * TODO: Check extension support.
 * TODO: Message callback in validation layers.
 * TODO: Device suitable checks.
 * 
 * TODO: Frames in flight
 * TODO: Swap chain recreation
 * TODO: Swap chain recreation of depth buffer
 * TODO: Vertex deduplication in loaded model
 * TODO: Support of index buffer by a loaded model
 * 
 * TODO: Staging buffer
 * TODO: Index buffer
 * TODO: Mipmaps
 * TODO: Multisampling
 * 
 * TODO: Automatic shader compilation
 * TODO: ini settings
 */

int main()
{
	try
	{
		Render render;
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
