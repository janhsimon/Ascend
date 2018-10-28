#pragma once

#include "Context.hpp"
#include "Swapchain.hpp"

namespace asc
{
	enum ShaderType
	{
		Vertex,
		Fragment
	};

	class Engine
	{
	private:
		std::unique_ptr<internal::Context> context;
		std::unique_ptr<internal::Swapchain> swapchain;

	public:
		Engine(const ApplicationInfo& applicationInfo);

		void loadShader(const std::string& filename, const ShaderType type);
	};
}