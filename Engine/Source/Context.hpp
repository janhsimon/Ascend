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
			vk::PhysicalDevice physicalDevice;
			uint32_t graphicsQueueFamilyIndex, presentQueueFamilyIndex;
			vk::Queue graphicsQueue, presentQueue;

			std::function<void(vk::Instance*)> destroyInstance;
			std::unique_ptr<vk::Instance, decltype(destroyInstance)> instance;
			
			std::function<void(vk::DebugUtilsMessengerEXT*)> destroyDebugMessenger;
			std::unique_ptr<vk::DebugUtilsMessengerEXT, decltype(destroyDebugMessenger)> debugMessenger;
						
			std::function<void(vk::SurfaceKHR*)> destroySurface;
			std::unique_ptr<vk::SurfaceKHR, decltype(destroySurface)> surface;

			std::function<void(vk::Device*)> destroyDevice;
			std::unique_ptr<vk::Device, decltype(destroyDevice)> device;

			std::function<void(vk::CommandPool*)> destroyCommandPool;
			std::unique_ptr<vk::CommandPool, decltype(destroyCommandPool)> commandPool;

			std::function<void(vk::Semaphore*)> destroySemaphore;
			std::unique_ptr<vk::Semaphore, decltype(destroySemaphore)> imageAvailableSemaphore, renderFinishedSemaphore;

			void createInstance();
			void createDebugMessenger();
			void createSurface();
			void selectPhysicalDevice();
			void selectQueueFamilyIndices();
			void createDevice();
			void retrieveQueues();
			void createCommandPool();
			void createSemaphores();

		public:
			Context(const asc::ApplicationInfo& _applicationInfo);

			ApplicationInfo& getApplicationInfo() { return applicationInfo; }
			vk::PhysicalDevice getPhysicalDevice() const { return physicalDevice; }
			uint32_t getGraphicsQueueFamilyIndex() const { return graphicsQueueFamilyIndex; }
			uint32_t getPresentQueueFamilyIndex() const { return presentQueueFamilyIndex; }
			vk::Queue getGraphicsQueue() const { return graphicsQueue; }
			vk::Queue getPresentQueue() const { return presentQueue; }
			vk::SurfaceKHR* getSurface() const { return surface.get(); }
			vk::Device* getDevice() const { return device.get(); }
			vk::CommandPool* getCommandPool() const { return commandPool.get(); }
			vk::Semaphore* getImageAvailableSemaphore() const { return imageAvailableSemaphore.get(); }
			vk::Semaphore* getRenderFinishedSemaphore() const { return renderFinishedSemaphore.get(); }
		};
	}
}