#pragma once

#include "Export.hpp"

#include <functional>
#include <vulkan/vulkan.hpp>

namespace asc
{
	struct ApplicationInfo
	{
		const char* name = "My Ascend Application";
		uint32_t versionMajor = 1, versionMinor = 0, versionPatch = 0;
		std::vector<const char*> instanceExtensions = {};
		std::function<void(const std::string&, const int)> logLambda = nullptr; // TODO: use LogSeverity instead of int
		std::function<VkSurfaceKHR*(const VkInstance*)> createSurfaceLambda = nullptr;

		ApplicationInfo& setName(const char* name)
		{
			this->name = name;
			return *this;
		}

		ApplicationInfo& setVersion(uint32_t versionMajor, uint32_t versionMinor, uint32_t versionPatch)
		{
			this->versionMajor = versionMajor;
			this->versionMinor = versionMinor;
			this->versionPatch = versionPatch;
			return *this;
		}

		ApplicationInfo& setInstanceExtensions(const std::vector<const char*> &instanceExtensions)
		{
			this->instanceExtensions = instanceExtensions;
			return *this;
		}
	};

	class ASC_EXPORT Application
	{
	private:
		static constexpr uint32_t API_VERSION = VK_API_VERSION_1_0;
		static constexpr char ENGINE_NAME[] = "Ascend";
		static constexpr uint32_t ENGINE_VERSION = VK_MAKE_VERSION(1, 0, 0);
		static constexpr char STANDARD_VALIDATION_LAYER_NAME[] = "VK_LAYER_LUNARG_standard_validation";

		const ApplicationInfo applicationInfo;

		std::function<void(vk::Instance*)> destroyInstance;
		std::unique_ptr<vk::Instance, decltype(destroyInstance)> instance;

	public:
		Application(const ApplicationInfo& _applicationInfo);

		ApplicationInfo getApplicationInfo() const { return applicationInfo; }
		VkInstance* getInstance() const { return reinterpret_cast<VkInstance*>(instance.get()); }
	};
}