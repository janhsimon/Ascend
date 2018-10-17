#pragma once

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
	}
}
