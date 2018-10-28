#include "Swapchain.hpp"

namespace asc
{
	namespace internal
	{
		void Swapchain::selectSurfaceFormat()
		{
			constexpr auto WANTED_SURFACE_FORMAT = vk::Format::eB8G8R8A8Unorm;
			constexpr auto WANTED_SURFACE_COLOR_SPACE = vk::ColorSpaceKHR::eSrgbNonlinear;

			const auto availableSurfaceFormats = context->getPhysicalDevice()->getSurfaceFormatsKHR(*context->getSurface());

			if (availableSurfaceFormats.size() == 1 && availableSurfaceFormats[0].format == vk::Format::eUndefined)
			// undefined as the only result means there are no preferences
			{
				surfaceFormat = WANTED_SURFACE_FORMAT;
				surfaceColorSpace = WANTED_SURFACE_COLOR_SPACE;
				return;
			}

			surfaceFormat = availableSurfaceFormats[0].format;
			surfaceColorSpace = availableSurfaceFormats[0].colorSpace;

			for (uint32_t i = 1; i < availableSurfaceFormats.size(); ++i)
			{
				const auto availableSurfaceFormat = availableSurfaceFormats[i];
				if (availableSurfaceFormat.format == WANTED_SURFACE_FORMAT && availableSurfaceFormat.colorSpace == WANTED_SURFACE_COLOR_SPACE)
				{
					surfaceFormat = WANTED_SURFACE_FORMAT;
					surfaceColorSpace = WANTED_SURFACE_COLOR_SPACE;
					return;
				}
			}
		}

		void Swapchain::selectPresentMode()
		{
			// fifo is guaranteed to be available and requires 2 images in the swapchain
			presentMode = vk::PresentModeKHR::eFifo;
			imageCount = 2;

			const auto availablePresentModes = context->getPhysicalDevice()->getSurfacePresentModesKHR(*context->getSurface());
			for (const auto& availablePresentMode : availablePresentModes)
			{
				if (presentMode == vk::PresentModeKHR::eMailbox)
				// prefer mailbox for triple buffering and add an extra swapchain image
				{
					presentMode = availablePresentMode;
					imageCount = 3;
					return;
				}
			}
		}

		void Swapchain::selectSwapExtent()
		{
			const auto surfaceCapabilities = context->getPhysicalDevice()->getSurfaceCapabilitiesKHR(*context->getSurface());
			extent = surfaceCapabilities.currentExtent;
		}

		void Swapchain::createSwapchain()
		{
			auto swapchainCreateInfo = vk::SwapchainCreateInfoKHR().setSurface(*context->getSurface()).setImageFormat(surfaceFormat).setImageColorSpace(surfaceColorSpace);
			swapchainCreateInfo.setPresentMode(presentMode).setMinImageCount(imageCount).setImageExtent(extent);
			swapchainCreateInfo.setImageArrayLayers(1).setClipped(true).setImageUsage(vk::ImageUsageFlagBits::eColorAttachment);

			const auto graphicsQueueFamilyIndex = context->getGraphicsQueueFamilyIndex();
			const auto presentQueueFamilyIndex = context->getPresentQueueFamilyIndex();

			if (graphicsQueueFamilyIndex != presentQueueFamilyIndex)
			{
				const std::vector<uint32_t> queueFamilyIndices = { graphicsQueueFamilyIndex, presentQueueFamilyIndex };
				swapchainCreateInfo.setQueueFamilyIndexCount(static_cast<uint32_t>(queueFamilyIndices.size())).setPQueueFamilyIndices(queueFamilyIndices.data());
				swapchainCreateInfo.setImageSharingMode(vk::SharingMode::eConcurrent);
			}

			const auto newSwapchain = new vk::SwapchainKHR(context->getDevice()->createSwapchainKHR(swapchainCreateInfo));

			destroySwapchain = [&](vk::SwapchainKHR* swapchain)
			{
				if (context && context->getDevice())
				{
					context->getDevice()->destroySwapchainKHR(*swapchain);
				}
			};

			swapchain = std::unique_ptr<vk::SwapchainKHR, decltype(destroySwapchain)>(newSwapchain, destroySwapchain);
		}

		void Swapchain::createImages()
		{
			images.clear();

			const auto retrievedImages = context->getDevice()->getSwapchainImagesKHR(*swapchain);
			images.reserve(retrievedImages.size());
			for (const auto& image : retrievedImages)
			{
				images.emplace_back(context->getDevice(), image, surfaceFormat);
			}
		}

		Swapchain::Swapchain(Context* _context)
			: context(_context)
		{
			selectSurfaceFormat();
			selectPresentMode();
			selectSwapExtent();
			createSwapchain();
			createImages();
		}
	}
}