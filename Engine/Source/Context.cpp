#include "Context.hpp"

namespace asc
{
	namespace internal
	{
		static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT *callbackData, void *userData)
		{
			const auto context = static_cast<Context*>(userData);
			context->getApplicationInfo().debugCallbackLambda(callbackData->pMessage);
			return VK_FALSE;
		}

		vk::Instance *Context::createInstance()
		{
			std::vector<const char *> layers;

			if (applicationInfo.debugMode)
			{
				layers.push_back("VK_LAYER_LUNARG_standard_validation");
			}

			std::vector<const char *> extensions(applicationInfo.instanceExtensionCount);
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

		vk::DebugUtilsMessengerEXT *Context::createDebugMessenger()
		{
			auto debugMessengerCreateInfo = vk::DebugUtilsMessengerCreateInfoEXT().setPUserData(this).setPfnUserCallback(debugCallback);
			debugMessengerCreateInfo.setMessageType(vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral | vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance | vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation);
			debugMessengerCreateInfo.setMessageSeverity(vk::DebugUtilsMessageSeverityFlagBitsEXT::eError | vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning | vk::DebugUtilsMessageSeverityFlagBitsEXT::eInfo | vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose);
			
			// extension functions must be called through a function pointer
			auto createDebugUtilsMessengerEXT = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(instance->getProcAddr(CREATE_DEBUG_MESSENGER_FUNCTION_NAME));
			VkDebugUtilsMessengerEXT debugMessenger;
			if (createDebugUtilsMessengerEXT(static_cast<VkInstance>(*instance.get()), &static_cast<VkDebugUtilsMessengerCreateInfoEXT>(debugMessengerCreateInfo), nullptr, &debugMessenger) != VK_SUCCESS)
			{
				throw std::runtime_error("Failed to create debug messenger.");
			}

			return new vk::DebugUtilsMessengerEXT(debugMessenger);
		}

		Context::Context(const asc::ApplicationInfo &applicationInfo)
		{
			this->applicationInfo = applicationInfo;

			instance = std::unique_ptr<vk::Instance, decltype(destroyInstance)>(createInstance(), destroyInstance);

			if (applicationInfo.debugMode)
			{
				debugMessenger = std::unique_ptr<vk::DebugUtilsMessengerEXT, decltype(destroyDebugMessenger)>(createDebugMessenger(), destroyDebugMessenger);
			}

			const auto appSurface = reinterpret_cast<vk::SurfaceKHR*>(applicationInfo.createSurfaceLambda(reinterpret_cast<VkInstance*>(instance.get())));
			surface = std::unique_ptr<vk::SurfaceKHR, decltype(destroySurface)>(appSurface, destroySurface);
		}
	}
}