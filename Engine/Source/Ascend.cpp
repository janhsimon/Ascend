#include "Ascend.hpp"

#include <vulkan/vulkan.hpp>

const uint32_t API_VERSION = VK_API_VERSION_1_0;
const char *ENGINE_NAME = "Ascend";
const uint32_t ENGINE_VERSION = VK_MAKE_VERSION(1, 0, 0);

namespace asc
{
	void createInstance(const InstanceCreateInfo &info)
	{
		std::vector<const char *> layers;

#ifdef _DEBUG
		layers.push_back("VK_LAYER_LUNARG_standard_validation");
		//layers.push_back("VK_LAYER_LUNARG_assistant_layer"); // TODO: causes vkCreateDevice() to crash (but vkCreateInstance is successful with it)
#endif

		std::vector<const char *> extensions(info.instanceExtensionCount);
		for (uint32_t i = 0; i < extensions.size(); ++i)
		{
			extensions[i] = info.instanceExtensions[i];
		}

#ifdef _DEBUG
		extensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
#endif

		auto applicationInfo = vk::ApplicationInfo().setApiVersion(API_VERSION).setPEngineName(ENGINE_NAME).setEngineVersion(ENGINE_VERSION);
		applicationInfo.setPApplicationName(info.applicationName).setApplicationVersion(VK_MAKE_VERSION(info.versionMajor, info.versionMinor, info.versionPatch));

		auto instanceCreateInfo = vk::InstanceCreateInfo().setPApplicationInfo(&applicationInfo).setEnabledLayerCount(static_cast<uint32_t>(layers.size()));
		instanceCreateInfo.setPpEnabledLayerNames(layers.data()).setEnabledExtensionCount(static_cast<uint32_t>(extensions.size())).setPpEnabledExtensionNames(extensions.data());

		auto instance = vk::createInstance(instanceCreateInfo);
	}
}