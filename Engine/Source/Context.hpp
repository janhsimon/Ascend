#pragma once

#include <functional>
#include <string>
#include <vector>
#include <vulkan/vulkan.hpp>

namespace asc
{
	struct ApplicationInfo
	{
		const char *name;
		uint32_t versionMajor, versionMinor, versionPatch;
		uint32_t instanceExtensionCount;
		const char* const* instanceExtensions = nullptr;
		std::function<VkSurfaceKHR(VkInstance)> createSurfaceLambda = nullptr;

		ApplicationInfo &setName(const char *name)
		{
			this->name = name;
			return *this;
		}

		ApplicationInfo &setVersion(uint32_t versionMajor, uint32_t versionMinor, uint32_t versionPatch)
		{
			this->versionMajor = versionMajor;
			this->versionMinor = versionMinor;
			this->versionPatch = versionPatch;
			return *this;
		}

		ApplicationInfo &setInstanceExtensionCount(uint32_t instanceExtensionCount)
		{
			this->instanceExtensionCount = instanceExtensionCount;
			return *this;
		}

		ApplicationInfo &setInstanceExtensions(const char* const* instanceExtensions)
		{
			this->instanceExtensions = instanceExtensions;
			return *this;
		}

		ApplicationInfo &setCreateSurfaceLambda(std::function<VkSurfaceKHR(VkInstance)> createSurfaceLambda)
		{
			this->createSurfaceLambda = createSurfaceLambda;
			return *this;
		}
	};

	class Context
	{
	private:
		vk::Instance *createInstance(const ApplicationInfo &appInfo);
		std::function<void(vk::Instance*)> destroyInstance = [](vk::Instance *instance) { if (instance) instance->destroy(); };
		std::unique_ptr<vk::Instance, decltype(destroyInstance)> instance;

		std::function<void(vk::SurfaceKHR*)> destroySurface = [this](vk::SurfaceKHR *surface) { if (instance) instance->destroySurfaceKHR(*surface); };
		std::unique_ptr<vk::SurfaceKHR, decltype(destroySurface)> surface;

	public:
		Context(const ApplicationInfo &appInfo);
	};
}