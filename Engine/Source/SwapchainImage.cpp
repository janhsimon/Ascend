#include "SwapchainImage.hpp"

namespace asc
{
	namespace internal
	{
		void SwapchainImage::createImageView(vk::Format swapchainSurfaceFormat)
		{
			auto imageViewCreateInfo = vk::ImageViewCreateInfo().setImage(image).setViewType(vk::ImageViewType::e2D).setFormat(swapchainSurfaceFormat);
			imageViewCreateInfo.setSubresourceRange(vk::ImageSubresourceRange(vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1));
			auto newImageView = new vk::ImageView(device->createImageView(imageViewCreateInfo));

			destroyImageView = [&](vk::ImageView* imageView)
			{
				if (device)
				{
					device->destroyImageView(*imageView);
				}
			};

			imageView = std::unique_ptr<vk::ImageView, decltype(destroyImageView)>(newImageView, destroyImageView);
		}

		SwapchainImage::SwapchainImage(vk::Device* _device, const vk::Image& _image, vk::Format swapchainSurfaceFormat)
			: device(_device), image(_image)
		{
			createImageView(swapchainSurfaceFormat);
		}
	}
}