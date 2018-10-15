#pragma once

#include <functional>
#include <vulkan/vulkan.hpp>

namespace asc
{
	struct ApplicationInfo
	{
		const char *name = "Untitled Game";
		uint32_t versionMajor = 1, versionMinor = 0, versionPatch = 0;
		uint32_t instanceExtensionCount = 0;
		const char *const *instanceExtensions = nullptr;
		std::function<VkSurfaceKHR*(VkInstance*)> createSurfaceLambda = nullptr;
		bool debugMode = false;
		std::function<void(const std::string &)> debugCallbackLambda = nullptr;

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

		ApplicationInfo &setCreateSurfaceLambda(std::function<VkSurfaceKHR*(VkInstance*)> createSurfaceLambda)
		{
			this->createSurfaceLambda = createSurfaceLambda;
			return *this;
		}

		ApplicationInfo &setDebugMode(bool debugMode)
		{
			this->debugMode = debugMode;
			return *this;
		}

		ApplicationInfo &setDebugCallbackLambda(std::function<void(const std::string &)> debugCallbackLambda)
		{
			this->debugCallbackLambda = debugCallbackLambda;
			return *this;
		}
	};
}