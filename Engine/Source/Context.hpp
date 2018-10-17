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

			void createInstance();
			std::function<void(vk::Instance*)> destroyInstance;
			std::unique_ptr<vk::Instance, decltype(destroyInstance)> instance;

			void createDebugMessenger();
			std::function<void(vk::DebugUtilsMessengerEXT*)> destroyDebugMessenger;
			std::unique_ptr<vk::DebugUtilsMessengerEXT, decltype(destroyDebugMessenger)> debugMessenger;
			
			void createSurface();
			std::function<void(vk::SurfaceKHR*)> destroySurface;
			std::unique_ptr<vk::SurfaceKHR, decltype(destroySurface)> surface;

			void selectPhysicalDevice();
			vk::PhysicalDevice physicalDevice;

			void selectQueueFamilyIndex();
			uint32_t queueFamilyIndex;

			void createDevice();
			std::function<void(vk::Device*)> destroyDevice;
			std::unique_ptr<vk::Device, decltype(destroyDevice)> device;

			void getQueue();
			vk::Queue queue;

		public:
			Context(const asc::ApplicationInfo& applicationInfo);

			ApplicationInfo& getApplicationInfo() { return applicationInfo; }
		};
	}
}