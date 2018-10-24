#include "Context.hpp"
#include "DebugCallback.hpp"

#include <optional>

namespace asc
{
	namespace internal
	{
		void Context::createInstance()
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
			const auto newInstance = new vk::Instance(vk::createInstance(instanceCreateInfo));

			destroyInstance = [](vk::Instance* instance)
			{
				if (instance)
				{
					instance->destroy();
				}
			};

			instance = std::unique_ptr<vk::Instance, decltype(destroyInstance)>(newInstance, destroyInstance);
		}

		void Context::createDebugMessenger()
		{
			auto debugMessengerCreateInfo = vk::DebugUtilsMessengerCreateInfoEXT().setPUserData(&applicationInfo).setPfnUserCallback(debugCallback);
			debugMessengerCreateInfo.setMessageType(vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral | vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance | vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation);
			debugMessengerCreateInfo.setMessageSeverity(vk::DebugUtilsMessageSeverityFlagBitsEXT::eError | vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning);
			
			VkDebugUtilsMessengerEXT cStyleDebugMessenger;
			const auto cStyleInstance = static_cast<VkInstance>(*instance.get());
			const auto cStyleCreateInfo = static_cast<VkDebugUtilsMessengerCreateInfoEXT>(debugMessengerCreateInfo);

			// extension functions must be called through a function pointer
			const auto createDebugUtilsMessengerEXT = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(instance->getProcAddr(CREATE_DEBUG_MESSENGER_FUNCTION_NAME));
			if (createDebugUtilsMessengerEXT(cStyleInstance, &cStyleCreateInfo, nullptr, &cStyleDebugMessenger) != VK_SUCCESS)
			{
				throw std::runtime_error("Failed to create debug messenger.");
			}
			const auto newDebugMessenger = new vk::DebugUtilsMessengerEXT(cStyleDebugMessenger);

			destroyDebugMessenger = [&](vk::DebugUtilsMessengerEXT* messenger)
			{
				if (instance)
				{
					// extension functions must be called through a function pointer
					auto destroyDebugUtilsMessengerEXT = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(instance->getProcAddr(DESTROY_DEBUG_MESSENGER_FUNCTION_NAME));
					destroyDebugUtilsMessengerEXT(*instance.get(), *messenger, nullptr);
				}
			};

			this->debugMessenger = std::unique_ptr<vk::DebugUtilsMessengerEXT, decltype(destroyDebugMessenger)>(newDebugMessenger, destroyDebugMessenger);
		}

		void Context::createSurface()
		{
			const auto cStyleInstance = static_cast<VkInstance>(*instance.get());
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
				throw std::runtime_error("Failed to find suitable physical device.");
			}

			for (auto& physicalDevice : physicalDevices)
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

				if (physicalDevice.getSurfaceSupportKHR(i, *surface.get()))
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

			throw std::runtime_error("Physical device does not provide required queues.");
		}

		void Context::createDevice()
		{
			const auto deviceFeatures = vk::PhysicalDeviceFeatures().setSamplerAnisotropy(true);
			const std::vector<const char*> deviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

			std::vector<vk::DeviceQueueCreateInfo> deviceQueueCreateInfos;

			constexpr float QUEUE_PRIORITY = 1.0f;
			auto deviceQueueCreateInfo = vk::DeviceQueueCreateInfo().setQueueFamilyIndex(graphicsQueueFamilyIndex);
			deviceQueueCreateInfo.setQueueCount(1).setPQueuePriorities(&QUEUE_PRIORITY);
			deviceQueueCreateInfos.push_back(deviceQueueCreateInfo);

			if (graphicsQueueFamilyIndex != presentQueueFamilyIndex)
			{
				deviceQueueCreateInfo.setQueueFamilyIndex(presentQueueFamilyIndex);
				deviceQueueCreateInfos.push_back(deviceQueueCreateInfo);
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

		Context::Context(const asc::ApplicationInfo& applicationInfo)
		{
			this->applicationInfo = applicationInfo;

			createInstance();
			
			if (applicationInfo.debugMode)
			{
				createDebugMessenger();
			}

			createSurface();
			selectPhysicalDevice();
			selectQueueFamilyIndices();
			createDevice();
			retrieveQueues();
		}
	}
}