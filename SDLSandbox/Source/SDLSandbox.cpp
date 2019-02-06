#include <Ascend.hpp>
#include <SDL.h>
#include <SDL_vulkan.h>
#include <stdlib.h> // for EXIT_SUCCESS and EXIT_FAILURE

class Game
{
private:
	static constexpr char APP_NAME[] = "Ascend SDL Sandbox";
	static constexpr auto WINDOW_WIDTH = 1280;
	static constexpr auto WINDOW_HEIGHT = 720;

	SDL_Window* window = nullptr;

	void createWindow()
	{
		if (SDL_Init(SDL_INIT_VIDEO) != 0)
		{
			throw std::runtime_error("Failed to initialize video subsystem, encountered error: " + std::string(SDL_GetError()));
		}

		window = SDL_CreateWindow(APP_NAME, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_VULKAN);
		if (!window)
		{
			throw std::runtime_error("Failed to create window, encountered error: " + std::string(SDL_GetError()));
		}
	}

	std::vector<const char*> queryInstanceExtensions()
	{
		unsigned int instanceExtensionCount = 0;
		if (!SDL_Vulkan_GetInstanceExtensions(window, &instanceExtensionCount, nullptr))
		{
			throw std::runtime_error("Failed to get instance extension count: " + std::string(SDL_GetError()));
		}

		std::vector<const char*> instanceExtensions(instanceExtensionCount);
		if (!SDL_Vulkan_GetInstanceExtensions(window, &instanceExtensionCount, instanceExtensions.data()))
		{
			throw std::runtime_error("Failed to get instance extensions: " + std::string(SDL_GetError()));
		}

		return instanceExtensions;
	}

public:
	Game()
	{
		createWindow();

		auto applicationInfo = asc::ApplicationInfo().setName(APP_NAME).setVersion(1, 0, 0).setInstanceExtensions(queryInstanceExtensions());

		#ifdef DEBUG
			applicationInfo.debugCallbackLambda = [](const std::string& message)
			{
				SDL_ShowSimpleMessageBox(SDL_MessageBoxFlags::SDL_MESSAGEBOX_ERROR, "Error", message.c_str(), nullptr);
			};
		#endif

		applicationInfo.createSurfaceLambda = [&](const VkInstance* instance) -> VkSurfaceKHR*
		{
			auto surface = new VkSurfaceKHR();
			if (!SDL_Vulkan_CreateSurface(window, *instance, surface))
			{
				throw std::runtime_error("Failed to create window surface.");
			}
			return surface;
		};

		const auto application = asc::Application(applicationInfo);
		auto renderer = asc::Renderer(application);

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

			renderer.renderFrame();
		}

		SDL_Quit();
	}
};

int main(int argc, char** argv)
{
	try
	{
		Game game;
	}
	catch (const std::exception& error)
	{
		SDL_ShowSimpleMessageBox(SDL_MessageBoxFlags::SDL_MESSAGEBOX_ERROR, "Error", error.what(), nullptr);
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}