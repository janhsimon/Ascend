#pragma once

#include "Context.hpp"

namespace asc
{
  namespace internal
  {
    class Shader
    {
    public:
      Shader(const vk::Device* device, const std::string& filename, vk::ShaderStageFlagBits shaderStageFlags);

      vk::PipelineShaderStageCreateInfo getPipelineShaderStageCreateInfo() const;

    private:
      static constexpr char SHADER_PROGRAM_ENTRY_POINT[] = "main";

      const vk::Device* device;
      vk::ShaderStageFlagBits shaderStageFlags;

      std::function<void(vk::ShaderModule*)> destroyShaderModule;
      std::unique_ptr<vk::ShaderModule, decltype(destroyShaderModule)> shaderModule;
    };
  }
}