#pragma once

#include "Context.hpp"

namespace asc
{
	namespace internal
	{
		class Swapchain
		{
		private:
			vk::Format surfaceFormat;
			vk::ColorSpaceKHR surfaceColorSpace;
			vk::PresentModeKHR presentMode;
			uint32_t imageCount;
			vk::Extent2D extent;

			std::function<void(vk::SwapchainKHR*)> destroySwapchain;
			std::unique_ptr<vk::SwapchainKHR, decltype(destroySwapchain)> swapchain;

			void selectSurfaceFormat(Context* context);
			void selectPresentMode(Context* context);
			void selectSwapExtent(Context* context);
			void createSwapchain(Context* context);

		public:
			Swapchain(Context* context);
		};
	}
}