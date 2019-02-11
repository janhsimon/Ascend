#include "Context.hpp"
#include "Log.hpp"

#include <optional>

namespace asc
{
	namespace internal
	{
		static VKAPI_ATTR VkBool32 VKAPI_CALL vulkanLogCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* callbackData, void* userData)
		{
			if (messageSeverity & VkDebugUtilsMessageSeverityFlagBitsEXT::VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT)
			{
				Log(callbackData->pMessage, LogSeverity::Error);
			}
			else if (messageSeverity & VkDebugUtilsMessageSeverityFlagBitsEXT::VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
			{
				Log(callbackData->pMessage, LogSeverity::Warning);
			}
			else
			{
				Log(callbackData->pMessage);
			}
			
			return VK_FALSE;
		}

		void Context::createDebugMessenger()
		{
			auto debugMessengerCreateInfo = vk::DebugUtilsMessengerCreateInfoEXT().setPfnUserCallback(vulkanLogCallback);
			debugMessengerCreateInfo.setMessageType(vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral | vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance | vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation);
			debugMessengerCreateInfo.setMessageSeverity(vk::DebugUtilsMessageSeverityFlagBitsEXT::eError | vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning);
			
			VkDebugUtilsMessengerEXT cStyleDebugMessenger;
			const auto cStyleInstance = static_cast<VkInstance>(*instance);
			const auto cStyleCreateInfo = static_cast<VkDebugUtilsMessengerCreateInfoEXT>(debugMessengerCreateInfo);

			// extension functions must be called through a function pointer
			const auto createDebugUtilsMessengerEXT = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(instance->getProcAddr(CREATE_DEBUG_MESSENGER_FUNCTION_NAME));
			if (createDebugUtilsMessengerEXT(cStyleInstance, &cStyleCreateInfo, nullptr, &cStyleDebugMessenger) != VK_SUCCESS)
			{
				Log("Failed to create debug messenger.", LogSeverity::Error);
			}
			const auto newDebugMessenger = new vk::DebugUtilsMessengerEXT(cStyleDebugMessenger);

			destroyDebugMessenger = [&](vk::DebugUtilsMessengerEXT* messenger)
			{
				if (instance)
				{
					// extension functions must be called through a function pointer
					auto destroyDebugUtilsMessengerEXT = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(instance->getProcAddr(DESTROY_DEBUG_MESSENGER_FUNCTION_NAME));
					destroyDebugUtilsMessengerEXT(*instance, *messenger, nullptr);
				}
			};

			debugMessenger = std::unique_ptr<vk::DebugUtilsMessengerEXT, decltype(destroyDebugMessenger)>(newDebugMessenger, destroyDebugMessenger);
		}
		
		void Context::createSurface()
		{
			const auto cStyleInstance = static_cast<VkInstance>(*instance);
			const auto appSurface = reinterpret_cast<vk::SurfaceKHR*>(applicationInfo.createSurfaceLambda(&cStyleInstance));

			destroySurface = [&](vk::SurfaceKHR* surface)
			{
				if (instance)
				{
					instance->destroySurfaceKHR(*surface);
				}
			};

			surface = std::unique_ptr<vk::SurfaceKHR, decltype(destroySurface)>(appSurface, destroySurface);
		}

		void Context::selectPhysicalDevice()
		{
			const auto physicalDevices = instance->enumeratePhysicalDevices();

			if (physicalDevices.empty())
			{
				Log("Failed to find suitable physical device.", LogSeverity::Fatal);
			}

			for (const auto& physicalDevice : physicalDevices)
			{
				// TODO: check that wanted device extensions are supported
				// TODO: check that swapchain support is adequate
				const auto properties = physicalDevice.getProperties();
				if (properties.deviceType == vk::PhysicalDeviceType::eDiscreteGpu)
				{
					this->physicalDevice = physicalDevice;
					return;
				}
			}

			physicalDevice = *physicalDevices.begin();
		}

		void Context::selectQueueFamilyIndices()
		{
			std::optional<uint32_t> selectedGraphicsQueueFamilyIndex, selectedPresentQueueFamilyIndex;

			const auto queueFamilies = physicalDevice.getQueueFamilyProperties();
			for (uint32_t i = 0; i < queueFamilies.size(); ++i)
			{
				const auto queueFamily = queueFamilies[i];

				if (queueFamily.queueCount <= 0)
				{
					continue;
				}

				if (queueFamily.queueFlags & vk::QueueFlagBits::eGraphics)
				{
					selectedGraphicsQueueFamilyIndex = i;
				}

				if (physicalDevice.getSurfaceSupportKHR(i, *surface))
				{
					selectedPresentQueueFamilyIndex = i;
				}

				if (selectedGraphicsQueueFamilyIndex.has_value() && selectedPresentQueueFamilyIndex.has_value())
				{
					graphicsQueueFamilyIndex = selectedGraphicsQueueFamilyIndex.value();
					presentQueueFamilyIndex = selectedPresentQueueFamilyIndex.value();
					return;
				}
			}

			Log("Physical device does not provide required queues.", LogSeverity::Fatal);
		}

		void Context::createDevice()
		{
			const auto deviceFeatures = vk::PhysicalDeviceFeatures().setSamplerAnisotropy(true);
			const std::vector<const char*> deviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

			std::vector<vk::DeviceQueueCreateInfo> deviceQueueCreateInfos;

			constexpr float QUEUE_PRIORITY = 1.0f;
			auto deviceQueueCreateInfo = vk::DeviceQueueCreateInfo().setQueueFamilyIndex(graphicsQueueFamilyIndex);
			deviceQueueCreateInfo.setQueueCount(1).setPQueuePriorities(&QUEUE_PRIORITY);
			deviceQueueCreateInfos.emplace_back(deviceQueueCreateInfo);

			if (graphicsQueueFamilyIndex != presentQueueFamilyIndex)
			{
				deviceQueueCreateInfo.setQueueFamilyIndex(presentQueueFamilyIndex);
				deviceQueueCreateInfos.emplace_back(deviceQueueCreateInfo);
			}

			auto deviceCreateInfo = vk::DeviceCreateInfo().setPEnabledFeatures(&deviceFeatures);
			deviceCreateInfo.setEnabledExtensionCount(static_cast<uint32_t>(deviceExtensions.size())).setPpEnabledExtensionNames(deviceExtensions.data());
			deviceCreateInfo.setQueueCreateInfoCount(static_cast<uint32_t>(deviceQueueCreateInfos.size())).setPQueueCreateInfos(deviceQueueCreateInfos.data());
			
			const auto newDevice = new vk::Device(physicalDevice.createDevice(deviceCreateInfo));

			destroyDevice = [](vk::Device* device)
			{
				if (device)
				{
					device->destroy();
				}
			};

			device = std::unique_ptr<vk::Device, decltype(destroyDevice)>(newDevice, destroyDevice);
		}

		void Context::retrieveQueues()
		{
			graphicsQueue = device->getQueue(graphicsQueueFamilyIndex, 0);
			presentQueue = device->getQueue(presentQueueFamilyIndex, 0);
		}

		void Context::createCommandPool()
		{
			auto commandPoolCreateInfo = vk::CommandPoolCreateInfo().setQueueFamilyIndex(graphicsQueueFamilyIndex);
			auto newCommandPool = new vk::CommandPool(device->createCommandPool(commandPoolCreateInfo));
			
			destroyCommandPool = [&](vk::CommandPool* commandPool)
			{
				if (device)
				{
					device->destroyCommandPool(*commandPool);
				}
			};
			
			commandPool = std::unique_ptr<vk::CommandPool, decltype(destroyCommandPool)>(newCommandPool, destroyCommandPool);
		}

		Context::Context(const vk::Instance* _instance, asc::ApplicationInfo& _applicationInfo)
			: instance(_instance), applicationInfo(_applicationInfo)
		{
			if (applicationInfo.debugMode)
			{
				createDebugMessenger();
			}

			createSurface();
			selectPhysicalDevice();
			selectQueueFamilyIndices();
			createDevice();
			retrieveQueues();
			createCommandPool();
		}
	}
}