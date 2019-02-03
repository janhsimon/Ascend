#pragma once

#include "Pipeline.hpp"
#include "Swapchain.hpp"

namespace asc
{
	enum ShaderType
	{
		Vertex,
		Fragment
	};

	class ASC_EXPORT Renderer
	{
	private:
		std::unique_ptr<internal::Context> context;
		std::unique_ptr<internal::Swapchain> swapchain;
		std::unique_ptr<internal::Pipeline> pipeline;

	public:
		Renderer(const Application& application);

		//void loadShader(const std::string& filename, const ShaderType type);
		void renderFrame() const;
	};
}