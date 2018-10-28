#pragma once

#include "Context.hpp"

namespace asc
{
	namespace internal
	{
		class SwapchainImage
		{
		private:
			vk::Device* device;
			vk::Image image;

			std::function<void(vk::ImageView*)> destroyImageView;
			std::unique_ptr<vk::ImageView, decltype(destroyImageView)> imageView;

			void createImageView(vk::Format swapchainSurfaceFormat);

		public:
			SwapchainImage(vk::Device* _device, const vk::Image &_image, vk::Format swapchainSurfaceFormat);
		};
	}
}
