#include <iostream>
#include <stdexcept>
#include <cstdlib>

#include "Application.h"

/**
 * TODO: Handle VK_ERROR_INCOMPATIBLE_DRIVER.
 * TODO: Check extension support.
 * TODO: Message callback in validation layers.
 * TODO: Device suitable checks.
 * 
 * TODO: Swap chain recreation (window resizing)
 * TODO: Swap chain recreation of depth buffer
 * TODO: Mipmaps
 * TODO: Multisampling
 */

int main()
{
	try
	{
		Application application;
	}
	catch (const std::exception& exception)
	{
		std::cerr << exception.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
