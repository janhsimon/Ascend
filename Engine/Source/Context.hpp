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
			static constexpr char STANDARD_VALIDATION_LAYER_NAME[] = "VK_LAYER_LUNARG_standard_validation";
			static constexpr char CREATE_DEBUG_MESSENGER_FUNCTION_NAME[] = "vkCreateDebugUtilsMessengerEXT";
			static constexpr char DESTROY_DEBUG_MESSENGER_FUNCTION_NAME[] = "vkDestroyDebugUtilsMessengerEXT";

			ApplicationInfo applicationInfo;

			vk::Instance* createInstance();
			std::function<void(vk::Instance*)> destroyInstance = [](vk::Instance* instance) { if (instance) instance->destroy(); };
			std::unique_ptr<vk::Instance, decltype(destroyInstance)> instance;

			vk::DebugUtilsMessengerEXT* createDebugMessenger();
			std::function<void(vk::DebugUtilsMessengerEXT*)> destroyDebugMessenger = [this](vk::DebugUtilsMessengerEXT* messenger)
			{
				if (instance)
				{
					// extension functions must be called through a function pointer
					auto destroyDebugUtilsMessengerEXT = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(instance->getProcAddr(DESTROY_DEBUG_MESSENGER_FUNCTION_NAME));
					destroyDebugUtilsMessengerEXT(*instance.get(), *messenger, nullptr);
				}
			};
			std::unique_ptr<vk::DebugUtilsMessengerEXT, decltype(destroyDebugMessenger)> debugMessenger;

			std::function<void(vk::SurfaceKHR*)> destroySurface = [this](vk::SurfaceKHR* surface) { if (instance) instance->destroySurfaceKHR(*surface); };
			std::unique_ptr<vk::SurfaceKHR, decltype(destroySurface)> surface;

			vk::PhysicalDevice* selectPhysicalDevice();
			std::unique_ptr<vk::PhysicalDevice> physicalDevice;

			vk::Device* createDevice();
			std::function<void(vk::Device*)> destroyDevice = [](vk::Device* device) { if (device) device->destroy(); };
			std::unique_ptr<vk::Device, decltype(destroyDevice)> device;

		public:
			Context(const asc::ApplicationInfo& applicationInfo);

			ApplicationInfo& getApplicationInfo() { return applicationInfo; }
		};
	}
}