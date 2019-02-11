#include "Log.hpp"
#include "Shader.hpp"

#include <fstream>

namespace asc
{
	namespace internal
	{
		Shader::Shader(const vk::Device* _device, const std::string& filename, vk::ShaderStageFlagBits _shaderStageFlags)
			: device(_device), shaderStageFlags(_shaderStageFlags)
		{
			std::ifstream file(filename, std::ios::binary);

			if (file.fail())
			{
				Log("Failed to open shader file \"" + filename + "\".", LogSeverity::Fatal);
			}

			const auto begin = file.tellg();
			file.seekg(0, std::ios::end);
			const auto end = file.tellg();

			std::vector<char> code(static_cast<size_t>(end - begin));
			file.seekg(0, std::ios::beg);
			file.read(&code[0], end - begin);
			file.close();

			if (code.size() == 0)
			{
				Log("Shader file \"" + filename + "\" is empty.", LogSeverity::Fatal);
			}

			const auto shaderModuleCreateInfo = vk::ShaderModuleCreateInfo().setCodeSize(code.size()).setPCode(reinterpret_cast<const uint32_t*>(&code[0]));
			const auto newShaderModule = new vk::ShaderModule(device->createShaderModule(shaderModuleCreateInfo));

			destroyShaderModule = [&](vk::ShaderModule* shaderModule)
			{
				if (device)
				{
					device->destroyShaderModule(*shaderModule);
				}
			};

			shaderModule = std::unique_ptr<vk::ShaderModule, decltype(destroyShaderModule)>(newShaderModule, destroyShaderModule);
		}

		vk::PipelineShaderStageCreateInfo Shader::getPipelineShaderStageCreateInfo() const
		{
			return vk::PipelineShaderStageCreateInfo().setStage(shaderStageFlags).setModule(*shaderModule.get()).setPName(SHADER_PROGRAM_ENTRY_POINT);
		}
	}
}