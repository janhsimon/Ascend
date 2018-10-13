#include "Engine.hpp"

namespace asc
{
	Engine::Engine(const ApplicationInfo &appInfo)
	{
		context = std::make_unique<internal::Context>(appInfo);
	}
}