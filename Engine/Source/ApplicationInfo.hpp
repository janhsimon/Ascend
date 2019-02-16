#pragma once

#include <functional>
#include <vulkan/vulkan.hpp>

namespace asc
{
  class ApplicationInfo
  {
  public:
    const char* name;
    uint32_t versionMajor, versionMinor, versionPatch;
    std::vector<const char*> instanceExtensions;
    bool debugMode;

    std::function<VkSurfaceKHR*(const VkInstance*)> createSurfaceLambda;
    ApplicationInfo();

    ApplicationInfo& setName(const char* name);
    ApplicationInfo& setVersion(const uint32_t versionMajor, const uint32_t versionMinor, const uint32_t versionPatch);
    ApplicationInfo& setInstanceExtensions(const std::vector<const char*> &instanceExtensions);
    ApplicationInfo& setDebugMode(const bool debugMode);
    ApplicationInfo& setCreateSurfaceLambda(std::function<VkSurfaceKHR*(const VkInstance*)> createSurfaceLambda);
  };
}