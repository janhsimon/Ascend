#pragma once

#include <functional>
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
}