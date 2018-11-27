#pragma once

#include "Context.hpp"

namespace asc
{
	namespace internal
	{
		class Shader
		{
		private:
			static constexpr char SHADER_PROGRAM_ENTRY_POINT[] = "main";

			const vk::Device* device;
			vk::ShaderStageFlagBits shaderStageFlags;

			std::function<void(vk::ShaderModule*)> destroyShaderModule;
			std::unique_ptr<vk::ShaderModule, decltype(destroyShaderModule)> shaderModule;

		public:
			Shader(const vk::Device* _device, const std::string& filename, vk::ShaderStageFlagBits _shaderStageFlags);

			vk::PipelineShaderStageCreateInfo getPipelineShaderStageCreateInfo() const;
		};
	}
}