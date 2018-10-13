#pragma once

#include "ApplicationInfo.hpp"

#include <string>
#include <vector>

namespace asc
{
	namespace internal
	{
		class Context
		{
		private:
			static constexpr uint32_t API_VERSION = VK_API_VERSION_1_0;
			static constexpr char ENGINE_NAME[] = "Ascend";
			static constexpr uint32_t ENGINE_VERSION = VK_MAKE_VERSION(1, 0, 0);

			vk::Instance *createInstance(const asc::ApplicationInfo &appInfo);
			std::function<void(vk::Instance*)> destroyInstance = [](vk::Instance *instance) { if (instance) instance->destroy(); };
			std::unique_ptr<vk::Instance, decltype(destroyInstance)> instance;

			std::function<void(vk::SurfaceKHR*)> destroySurface = [this](vk::SurfaceKHR *surface) { if (instance) instance->destroySurfaceKHR(*surface); };
			std::unique_ptr<vk::SurfaceKHR, decltype(destroySurface)> surface;

		public:
			Context(const asc::ApplicationInfo &appInfo);
		};
	}
}