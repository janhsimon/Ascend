#include <Ascend.hpp>
#include <SDL.h>
#include <SDL_vulkan.h>
#include <stdlib.h> // for EXIT_SUCCESS and EXIT_FAILURE

class Game
{
private:
	SDL_Window *window;

public:
	Game()
	{
		if (SDL_Init(SDL_INIT_VIDEO) != 0)
		{
			throw std::runtime_error("Failed to initialize video subsystem, encountered error: " + std::string(SDL_GetError()));
		}

		window = SDL_CreateWindow("Ascend Test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_VULKAN);
		if (!window)
		{
			throw std::runtime_error("Failed to create window, encountered error: " + std::string(SDL_GetError()));
		}

		unsigned int instanceExtensionCount = 0;
		if (!SDL_Vulkan_GetInstanceExtensions(window, &instanceExtensionCount, nullptr))
		{
			throw std::runtime_error("Failed to get instance extension count: " + std::string(SDL_GetError()));
		}

		std::vector<const char *> instanceExtensions(instanceExtensionCount);
		if (!SDL_Vulkan_GetInstanceExtensions(window, &instanceExtensionCount, instanceExtensions.data()))
		{
			throw std::runtime_error("Failed to get instance extensions: " + std::string(SDL_GetError()));
		}

		auto appInfo = asc::ApplicationInfo().setName("Ascend Test").setVersion(1, 0, 0);
		appInfo.setInstanceExtensionCount(static_cast<uint32_t>(instanceExtensions.size())).setInstanceExtensions(instanceExtensions.data());

		#ifdef NDEBUG
			appInfo.setDebugMode(false);
		#else
			appInfo.setDebugMode(true);
		#endif
		
		appInfo.createSurfaceLambda = [=](VkInstance *instance) -> VkSurfaceKHR*
		{
			auto surface = new VkSurfaceKHR();
			if (!SDL_Vulkan_CreateSurface(window, *instance, surface))
			{
				throw std::runtime_error("Failed to create window surface.");
			}
			return surface;
		};
		
		auto engine = asc::Engine(appInfo);

		SDL_Event event;
		auto done = false;
		while (true)
		{
			while (SDL_PollEvent(&event))
			{
				if (event.type == SDL_QUIT || (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_ESCAPE))
				{
					done = true;
					break;
				}
			}

			if (done)
			{
				break;
			}

			// TODO: do something here...
		}

		SDL_Quit();
	}
};

int main(int argc, char **argv)
{
	try
	{
		Game game;
	}
	catch (const std::exception &error)
	{
		SDL_ShowSimpleMessageBox(SDL_MessageBoxFlags::SDL_MESSAGEBOX_ERROR, "Error", error.what(), nullptr);
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}