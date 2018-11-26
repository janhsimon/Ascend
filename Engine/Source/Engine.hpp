#pragma once

#include "Context.hpp"
#include "Swapchain.hpp"

#ifdef ASC_BUILD_DLL
	#define ASC_EXPORT __declspec(dllexport)
#else
	#define ASC_EXPORT __declspec(dllimport)
#endif

namespace asc
{
	enum ShaderType
	{
		Vertex,
		Fragment
	};

	class ASC_EXPORT Engine
	{
	private:
		std::unique_ptr<internal::Context> context;
		std::unique_ptr<internal::Swapchain> swapchain;

	public:
		Engine(const ApplicationInfo& applicationInfo);

		void loadShader(const std::string& filename, const ShaderType type);
	};
}