#pragma once

#include <functional>
#include <vulkan/vulkan.hpp>

namespace asc
{
	struct ApplicationInfo
	{
		const char* name = "Untitled Game";
		uint32_t versionMajor = 1, versionMinor = 0, versionPatch = 0;
		std::vector<const char*> instanceExtensions = {};
		std::function<VkSurfaceKHR*(const VkInstance*)> createSurfaceLambda = nullptr;
		std::function<void(const std::string&)> debugCallbackLambda = nullptr;

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

		ApplicationInfo& setCreateSurfaceLambda(std::function<VkSurfaceKHR*(const VkInstance*)> createSurfaceLambda)
		{
			this->createSurfaceLambda = createSurfaceLambda;
			return *this;
		}

		ApplicationInfo& setDebugCallbackLambda(std::function<void(const std::string&)> debugCallbackLambda)
		{
			this->debugCallbackLambda = debugCallbackLambda;
			return *this;
		}
	};
}