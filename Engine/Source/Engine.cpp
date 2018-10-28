#include "Engine.hpp"
#include "Shader.hpp"

namespace asc
{
	Engine::Engine(const ApplicationInfo& applicationInfo)
	{
		context = std::make_unique<internal::Context>(applicationInfo);
		swapchain = std::make_unique<internal::Swapchain>(context.get());
	}

	void Engine::loadShader(const std::string& filename, const ShaderType type)
	{
		auto flags = vk::ShaderStageFlagBits::eVertex;

		if (type == ShaderType::Fragment)
		{
			flags = vk::ShaderStageFlagBits::eFragment;
		}

		asc::internal::Shader vertexShader(context->getDevice(), filename, flags);
	}
}