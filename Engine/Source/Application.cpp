#include "../Include/Ascend.hpp"

namespace asc
{
  Application::Application(const ApplicationInfo& applicationInfo)
  {
    this->applicationInfo = std::make_unique<ApplicationInfo>(applicationInfo);

    auto vulkanInfo = vk::ApplicationInfo().setApiVersion(API_VERSION).setPEngineName(ENGINE_NAME).setEngineVersion(ENGINE_VERSION);
    vulkanInfo.setApplicationVersion(VK_MAKE_VERSION(applicationInfo.versionMajor, applicationInfo.versionMinor, applicationInfo.versionPatch));
    vulkanInfo.setPApplicationName(applicationInfo.name);

    auto instanceCreateInfo = vk::InstanceCreateInfo().setPApplicationInfo(&vulkanInfo);

    const std::vector<const char*> layers = { STANDARD_VALIDATION_LAYER_NAME };
    std::vector<const char*> extensions(applicationInfo.instanceExtensions);

    if (applicationInfo.debugMode)
    {
      instanceCreateInfo.setEnabledLayerCount(static_cast<uint32_t>(layers.size())).setPpEnabledLayerNames(layers.data());
      extensions.emplace_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }

    instanceCreateInfo.setEnabledExtensionCount(static_cast<uint32_t>(extensions.size())).setPpEnabledExtensionNames(extensions.data());
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
}