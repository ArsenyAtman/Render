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
 * TODO: WindowManager
 * TODO: InstanceManager
 * TODO: SurfaceManager
 * TODO: PhysicalDeviceManager
 * TODO: LogicalDeviceManager
 * TODO: SwapChainManager
 * TODO: ImageViewsManager
 */

int main()
{
	Render render;

	//try
	{
		render.run();
	}
	//catch (const std::exception& e)
	{
		//std::cerr << e.what() << std::endl;
		//return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
