#pragma once

#include "ApplicationInfo.hpp"

namespace asc
{
  class Application
  {
  public:
    Application(const ApplicationInfo& applicationInfo);

    ApplicationInfo* getApplicationInfo() const { return applicationInfo.get(); }
    VkInstance* getInstance() const { return reinterpret_cast<VkInstance*>(instance.get()); }

  private:
    static constexpr uint32_t API_VERSION = VK_API_VERSION_1_0;
    static constexpr char ENGINE_NAME[] = "Ascend";
    static constexpr uint32_t ENGINE_VERSION = VK_MAKE_VERSION(1, 0, 0);
    static constexpr char STANDARD_VALIDATION_LAYER_NAME[] = "VK_LAYER_LUNARG_standard_validation";

    std::unique_ptr<ApplicationInfo> applicationInfo;

    std::function<void(vk::Instance*)> destroyInstance;
    std::unique_ptr<vk::Instance, decltype(destroyInstance)> instance;
  };
}