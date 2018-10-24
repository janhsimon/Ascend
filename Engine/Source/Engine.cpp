#include "Engine.hpp"

namespace asc
{
	Engine::Engine(const ApplicationInfo& applicationInfo)
	{
		context = std::make_unique<internal::Context>(applicationInfo);
		swapchain = std::make_unique<internal::Swapchain>(context.get());
	}
}