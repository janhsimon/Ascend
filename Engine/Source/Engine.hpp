#pragma once

#include "Context.hpp"
#include "Swapchain.hpp"

namespace asc
{
	class Engine
	{
	private:
		std::unique_ptr<internal::Context> context;
		std::unique_ptr<internal::Swapchain> swapchain;

	public:
		Engine(const ApplicationInfo& applicationInfo);
	};
}