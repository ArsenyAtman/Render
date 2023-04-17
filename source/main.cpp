#include <iostream>
#include <stdexcept>
#include <cstdlib>

#include "Application.h"

/**
 * TODO: Frames in flight
 * TODO: Staging buffer
 * TODO: Index buffer
 * TODO: Swap chain recreation
 * TODO: Swap chain recreation of depth buffer
 * TODO: Vertex deduplication in loaded model
 * TODO: Support of index buffer by a loaded model
 * 
 * TODO: Model loader
 * TODO: Automatic shader compilation
 * TODO: ini settings
 * 
 * TODO: Handle VK_ERROR_INCOMPATIBLE_DRIVER.
 * TODO: Check extension support.
 * TODO: Message callback in validation layers.
 * TODO: Device suitable checks.
 * 
 * TODO: Mipmaps
 * TODO: Multisampling
 */

int main()
{
	try
	{
		Application application;
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
