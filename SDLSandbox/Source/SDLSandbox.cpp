#include <Ascend.hpp>
#include <iostream>
#include <SDL.h>
#include <SDL_vulkan.h>

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
			asc::Log("Failed to initialize video subsystem, encountered error: " + std::string(SDL_GetError()), asc::LogSeverity::Fatal);
		}

		window = SDL_CreateWindow(APP_NAME, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_VULKAN);
		if (!window)
		{
			asc::Log("Failed to create window, encountered error: " + std::string(SDL_GetError()), asc::LogSeverity::Fatal);
		}
	}

	std::vector<const char*> queryInstanceExtensions()
	{
		unsigned int instanceExtensionCount = 0;
		if (!SDL_Vulkan_GetInstanceExtensions(window, &instanceExtensionCount, nullptr))
		{
			asc::Log("Failed to get instance extension count: " + std::string(SDL_GetError()), asc::LogSeverity::Fatal);
		}

		std::vector<const char*> instanceExtensions(instanceExtensionCount);
		if (!SDL_Vulkan_GetInstanceExtensions(window, &instanceExtensionCount, instanceExtensions.data()))
		{
			asc::Log("Failed to get instance extensions: " + std::string(SDL_GetError()), asc::LogSeverity::Fatal);
		}

		return instanceExtensions;
	}

public:
	Game()
	{
		const auto logLambda = [](const std::string& message, const asc::LogSeverity severity)
		{
			std::cout << message << std::endl;

			if (severity == asc::LogSeverity::Warning)
			{
				SDL_ShowSimpleMessageBox(SDL_MessageBoxFlags::SDL_MESSAGEBOX_INFORMATION, "Warning", message.c_str(), nullptr);
			}
			else if (severity == asc::LogSeverity::Error)
			{
				SDL_ShowSimpleMessageBox(SDL_MessageBoxFlags::SDL_MESSAGEBOX_WARNING, "Error", message.c_str(), nullptr);
			}
			else if (severity >= asc::LogSeverity::Fatal)
			{
				SDL_ShowSimpleMessageBox(SDL_MessageBoxFlags::SDL_MESSAGEBOX_ERROR, "Fatal Error", message.c_str(), nullptr);
				SDL_Quit();
				exit(1);
			}
		};

		asc::SetLogLambda(logLambda);

		createWindow();

		auto applicationInfo = asc::ApplicationInfo().setName(APP_NAME).setVersion(1, 0, 0).setInstanceExtensions(queryInstanceExtensions());

		#ifdef DEBUG
			applicationInfo.setDebugMode(true);
		#endif

		applicationInfo.createSurfaceLambda = [&](const VkInstance* instance) -> VkSurfaceKHR*
		{
			auto surface = new VkSurfaceKHR();
			if (!SDL_Vulkan_CreateSurface(window, *instance, surface))
			{
				asc::Log("Failed to create window surface.", asc::LogSeverity::Fatal);
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
	Game game;
	return 0;
}