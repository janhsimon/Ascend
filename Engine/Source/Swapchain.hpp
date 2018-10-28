#pragma once

#include "SwapchainImage.hpp"

namespace asc
{
	namespace internal
	{
		class Swapchain
		{
		private:
			Context* context;
			vk::Format surfaceFormat;
			vk::ColorSpaceKHR surfaceColorSpace;
			vk::PresentModeKHR presentMode;
			uint32_t imageCount;
			vk::Extent2D extent;
			std::vector<SwapchainImage> images;

			std::function<void(vk::SwapchainKHR*)> destroySwapchain;
			std::unique_ptr<vk::SwapchainKHR, decltype(destroySwapchain)> swapchain;

			void selectSurfaceFormat();
			void selectPresentMode();
			void selectSwapExtent();
			void createSwapchain();
			void createImages();

		public:
			Swapchain(Context* _context);
		};
	}
}