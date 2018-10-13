#pragma once

#include "Context.hpp"

namespace asc
{
	class Engine
	{
	private:
		std::unique_ptr<internal::Context> context;

	public:
		Engine(const ApplicationInfo &appInfo);
	};
}