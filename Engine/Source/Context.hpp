#pragma once

#include "Application.hpp"

namespace asc
{
	namespace internal
	{
		class Context
		{
		private:
			static constexpr char CREATE_DEBUG_MESSENGER_FUNCTION_NAME[] = "vkCreateDebugUtilsMessengerEXT";
			static constexpr char DESTROY_DEBUG_MESSENGER_FUNCTION_NAME[] = "vkDestroyDebugUtilsMessengerEXT";

			const vk::Instance* instance;
			ApplicationInfo applicationInfo;

			vk::PhysicalDevice physicalDevice;
			uint32_t graphicsQueueFamilyIndex, presentQueueFamilyIndex;
			vk::Queue graphicsQueue, presentQueue;
			
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

			void createDebugMessenger();
			void createSurface();
			void selectPhysicalDevice();
			void selectQueueFamilyIndices();
			void createDevice();
			void retrieveQueues();
			void createCommandPool();
			void createSemaphores();

		public:
			Context(const vk::Instance* _instance, asc::ApplicationInfo& _applicationInfo);

			//ApplicationInfo& getApplicationInfo() { return applicationInfo; }
			//vk::Instance* getInstance() const { return instance.get(); }
			//const vk::SurfaceKHR* getSurface() const { return surface; }
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