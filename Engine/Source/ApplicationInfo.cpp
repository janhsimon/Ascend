#include "ApplicationInfo.hpp"

namespace asc
{
  ApplicationInfo::ApplicationInfo() :
    name("My Ascend Application"),
    versionMajor(1u), versionMinor(0u), versionPatch(0u),
    instanceExtensions({}),
    debugMode(false),
    createSurfaceLambda(nullptr)
  {
    
  }

  ApplicationInfo& ApplicationInfo::setName(const char* name)
  {
    this->name = name;
    return *this;
  }

  ApplicationInfo& ApplicationInfo::setVersion(const uint32_t versionMajor, const uint32_t versionMinor, const uint32_t versionPatch)
  {
    this->versionMajor = versionMajor;
    this->versionMinor = versionMinor;
    this->versionPatch = versionPatch;
    return *this;
  }

  ApplicationInfo& ApplicationInfo::setInstanceExtensions(const std::vector<const char*> &instanceExtensions)
  {
    this->instanceExtensions = instanceExtensions;
    return *this;
  }

  ApplicationInfo& ApplicationInfo::setDebugMode(const bool debugMode)
  {
    this->debugMode = debugMode;
    return *this;
  }

  ApplicationInfo& ApplicationInfo::setCreateSurfaceLambda(std::function<VkSurfaceKHR*(const VkInstance*)> createSurfaceLambda)
  {
    this->createSurfaceLambda = createSurfaceLambda;
    return *this;
  }
}