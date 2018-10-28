#pragma once

#include "Context.hpp"

namespace asc
{
	namespace internal
	{
		class Shader
		{
		private:
			vk::Device* device;
			vk::ShaderStageFlagBits shaderStageFlags;

			std::function<void(vk::ShaderModule*)> destroyShaderModule;// = [this](vk::ShaderModule *shaderModule) { if (context->getDevice()) context->getDevice()->destroyShaderModule(*shaderModule); };
			std::unique_ptr<vk::ShaderModule, decltype(destroyShaderModule)> shaderModule;

		public:
			Shader(vk::Device* _device, const std::string& filename, vk::ShaderStageFlagBits _shaderStageFlags);
		};
	}
}