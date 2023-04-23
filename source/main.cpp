#include <iostream>
#include <stdexcept>
#include <cstdlib>

#include "Application.h"

/**
 * TODOOO: wrap currentImage into getters of the classes (in CommandManager too)!!!
 * TODO00: make Buffers part of CommandManager
 * TODO)O: make UniformBuffer and Texture part of DescriptorsManager
 * TODOOP: make DescriptorsManager part of GraphicsPipeline
 * TOOO: move swap chain related things from CommandManager to swap chain ???
 * TODO: rename CommandManager to command buffer
 * TODO: tickable interface
 * TODO: refactor helpers
 * 
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
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
