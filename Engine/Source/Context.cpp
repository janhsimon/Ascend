#include "Context.hpp"

namespace asc
{
	namespace internal
	{
		vk::Instance *Context::createInstance(const asc::ApplicationInfo &appInfo)
		{
			std::vector<const char *> layers;

			if (appInfo.debugMode)
			{
				layers.push_back("VK_LAYER_LUNARG_standard_validation");
				//layers.push_back("VK_LAYER_LUNARG_assistant_layer"); // TODO: causes vkCreateDevice() to crash (but vkCreateInstance is successful with it)
			}

			std::vector<const char *> extensions(appInfo.instanceExtensionCount);
			for (uint32_t i = 0; i < extensions.size(); ++i)
			{
				extensions[i] = appInfo.instanceExtensions[i];
			}

			if (appInfo.debugMode)
			{
				extensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
			}

			auto applicationInfo = vk::ApplicationInfo().setApiVersion(API_VERSION).setPEngineName(ENGINE_NAME).setEngineVersion(ENGINE_VERSION);
			applicationInfo.setPApplicationName(appInfo.name).setApplicationVersion(VK_MAKE_VERSION(appInfo.versionMajor, appInfo.versionMinor, appInfo.versionPatch));

			auto instanceCreateInfo = vk::InstanceCreateInfo().setPApplicationInfo(&applicationInfo).setEnabledLayerCount(static_cast<uint32_t>(layers.size()));
			instanceCreateInfo.setPpEnabledLayerNames(layers.data()).setEnabledExtensionCount(static_cast<uint32_t>(extensions.size())).setPpEnabledExtensionNames(extensions.data());
			return new vk::Instance(vk::createInstance(instanceCreateInfo));
		}

		Context::Context(const asc::ApplicationInfo &appInfo)
		{
			instance = std::unique_ptr<vk::Instance, decltype(destroyInstance)>(createInstance(appInfo), destroyInstance);
			surface = std::unique_ptr<vk::SurfaceKHR, decltype(destroySurface)>(reinterpret_cast<vk::SurfaceKHR*>(appInfo.createSurfaceLambda(*instance.get())), destroySurface);
		}
	}
}