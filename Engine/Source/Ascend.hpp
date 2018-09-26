#pragma once

#include <string>
#include <vector>

namespace asc
{
	struct InstanceCreateInfo
	{
		const char *applicationName;
		uint32_t versionMajor, versionMinor, versionPatch;
		uint32_t instanceExtensionCount;
		const char* const* instanceExtensions = nullptr;

		InstanceCreateInfo &setApplicationName(const char *applicationName)
		{
			this->applicationName = applicationName;
			return *this;
		}

		InstanceCreateInfo &setVersion(uint32_t versionMajor, uint32_t versionMinor, uint32_t versionPatch)
		{
			this->versionMajor = versionMajor;
			this->versionMinor = versionMinor;
			this->versionPatch = versionPatch;
			return *this;
		}

		InstanceCreateInfo &setInstanceExtensionCount(uint32_t instanceExtensionCount)
		{
			this->instanceExtensionCount = instanceExtensionCount;
			return *this;
		}

		InstanceCreateInfo& setInstanceExtensions(const char* const* instanceExtensions)
		{
			this->instanceExtensions = instanceExtensions;
			return *this;
		}
	};

	void createInstance(const InstanceCreateInfo &info);
}