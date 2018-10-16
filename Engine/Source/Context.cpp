#include "Context.hpp"

namespace asc
{
	namespace internal
	{
		static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* callbackData, void* userData)
		{
			const auto context = static_cast<Context*>(userData);
			context->getApplicationInfo().debugCallbackLambda(callbackData->pMessage);
			return VK_FALSE;
		}

		vk::Instance* Context::createInstance()
		{
			std::vector<const char*> layers;

			if (applicationInfo.debugMode)
			{
				layers.push_back(STANDARD_VALIDATION_LAYER_NAME);
			}

			std::vector<const char*> extensions(applicationInfo.instanceExtensionCount);
			for (uint32_t i = 0; i < extensions.size(); ++i)
			{
				extensions[i] = applicationInfo.instanceExtensions[i];
			}

			if (applicationInfo.debugMode)
			{
				extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
			}

			auto appInfo = vk::ApplicationInfo().setApiVersion(API_VERSION).setPEngineName(ENGINE_NAME).setEngineVersion(ENGINE_VERSION);
			appInfo.setApplicationVersion(VK_MAKE_VERSION(applicationInfo.versionMajor, applicationInfo.versionMinor, applicationInfo.versionPatch));
			appInfo.setPApplicationName(applicationInfo.name);

			auto instanceCreateInfo = vk::InstanceCreateInfo().setEnabledLayerCount(static_cast<uint32_t>(layers.size())).setPpEnabledLayerNames(layers.data());
			instanceCreateInfo.setEnabledExtensionCount(static_cast<uint32_t>(extensions.size())).setPpEnabledExtensionNames(extensions.data());
			instanceCreateInfo.setPApplicationInfo(&appInfo);
			return new vk::Instance(vk::createInstance(instanceCreateInfo));
		}

		vk::DebugUtilsMessengerEXT* Context::createDebugMessenger()
		{
			if (!applicationInfo.debugMode)
			{
				return nullptr;
			}

			auto debugMessengerCreateInfo = vk::DebugUtilsMessengerCreateInfoEXT().setPUserData(this).setPfnUserCallback(debugCallback);
			debugMessengerCreateInfo.setMessageType(vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral | vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance | vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation);
			debugMessengerCreateInfo.setMessageSeverity(vk::DebugUtilsMessageSeverityFlagBitsEXT::eError | vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning);
			
			VkDebugUtilsMessengerEXT debugMessenger;
			const auto cStyleInstance = static_cast<VkInstance>(*instance.get());
			const auto cStyleCreateInfo = static_cast<VkDebugUtilsMessengerCreateInfoEXT>(debugMessengerCreateInfo);

			// extension functions must be called through a function pointer
			const auto createDebugUtilsMessengerEXT = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(instance->getProcAddr(CREATE_DEBUG_MESSENGER_FUNCTION_NAME));
			if (createDebugUtilsMessengerEXT(cStyleInstance, &cStyleCreateInfo, nullptr, &debugMessenger) != VK_SUCCESS)
			{
				throw std::runtime_error("Failed to create debug messenger.");
			}

			return new vk::DebugUtilsMessengerEXT(debugMessenger);
		}

		vk::PhysicalDevice* Context::selectPhysicalDevice()
		{
			auto physicalDevices = instance->enumeratePhysicalDevices();

			if (!physicalDevices.empty())
			{
				throw std::runtime_error("Failed to find suitable physical device.");
			}

			for (const auto& physicalDevice : physicalDevices)
			{
				const auto physicalDeviceProperties = physicalDevice.getProperties();
				if (physicalDeviceProperties.deviceType == vk::PhysicalDeviceType::eDiscreteGpu)
				{
					return new vk::PhysicalDevice(physicalDevice);
				}
			}

			return new vk::PhysicalDevice(*physicalDevices.begin());
		}

		vk::Device* Context::createDevice()
		{
			const auto queueFamilyProperties = physicalDevice->getQueueFamilyProperties();

			auto queueFamilyIndex = -1;
			for (uint32_t i = 0; i < queueFamilyProperties.size(); ++i)
			{
				if (queueFamilyProperties[i].queueCount > 0 && queueFamilyProperties[i].queueFlags & vk::QueueFlagBits::eGraphics)
				{
					if (physicalDevice->getSurfaceSupportKHR(i, *surface.get()))
					{
						queueFamilyIndex = i;
						break;
					}
				}
			}

			if (queueFamilyIndex < 0)
			{
				throw std::runtime_error("Failed to find suitable queue family for physical device.");
			}

			const std::vector<float> queuePriorities = { 1.0f };
			auto deviceQueueCreateInfo = vk::DeviceQueueCreateInfo().setQueueFamilyIndex(queueFamilyIndex);
			deviceQueueCreateInfo.setQueueCount(static_cast<uint32_t>(queuePriorities.size())).setPQueuePriorities(queuePriorities.data());

			const auto deviceFeatures = vk::PhysicalDeviceFeatures().setSamplerAnisotropy(true);

			const std::vector<const char*> deviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
			
			auto deviceCreateInfo = vk::DeviceCreateInfo().setQueueCreateInfoCount(1).setPQueueCreateInfos(&deviceQueueCreateInfo).setPEnabledFeatures(&deviceFeatures);
			deviceCreateInfo.setEnabledExtensionCount(static_cast<uint32_t>(deviceExtensions.size())).setPpEnabledExtensionNames(deviceExtensions.data());
			return new vk::Device(physicalDevice->createDevice(deviceCreateInfo));
		}

		Context::Context(const asc::ApplicationInfo& applicationInfo)
		{
			this->applicationInfo = applicationInfo;

			instance = std::unique_ptr<vk::Instance, decltype(destroyInstance)>(createInstance(), destroyInstance);
			debugMessenger = std::unique_ptr<vk::DebugUtilsMessengerEXT, decltype(destroyDebugMessenger)>(createDebugMessenger(), destroyDebugMessenger);

			const auto cStyleInstance = static_cast<VkInstance>(*instance.get());
			const auto appSurface = reinterpret_cast<vk::SurfaceKHR*>(applicationInfo.createSurfaceLambda(&cStyleInstance));
			surface = std::unique_ptr<vk::SurfaceKHR, decltype(destroySurface)>(appSurface, destroySurface);

			physicalDevice = std::unique_ptr<vk::PhysicalDevice>(selectPhysicalDevice());

			device = std::unique_ptr<vk::Device, decltype(destroyDevice)>(createDevice(), destroyDevice);
		}
	}
}