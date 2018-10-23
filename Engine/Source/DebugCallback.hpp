#pragma once

#include "ApplicationInfo.hpp"

namespace asc
{
	namespace internal
	{
		static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* callbackData, void* userData)
		{
			const auto applicationInfo = static_cast<ApplicationInfo*>(userData);
			applicationInfo->debugCallbackLambda(callbackData->pMessage);
			return VK_FALSE;
		}
	}
}
