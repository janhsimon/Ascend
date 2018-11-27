#include "SwapchainImage.hpp"

namespace asc
{
	namespace internal
	{
		void SwapchainImage::createImageView()
		{
			auto imageViewCreateInfo = vk::ImageViewCreateInfo().setImage(image).setViewType(vk::ImageViewType::e2D).setFormat(swapchainSurfaceFormat);
			imageViewCreateInfo.setSubresourceRange(vk::ImageSubresourceRange(vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1));
			auto newImageView = new vk::ImageView(context->getDevice()->createImageView(imageViewCreateInfo));

			destroyImageView = [&](vk::ImageView* imageView)
			{
				if (context && context->getDevice())
				{
					context->getDevice()->destroyImageView(*imageView);
				}
			};

			imageView = std::unique_ptr<vk::ImageView, decltype(destroyImageView)>(newImageView, destroyImageView);
		}

		void SwapchainImage::createFramebuffer()
		{
			auto framebufferCreateInfo = vk::FramebufferCreateInfo().setRenderPass(*swapchainRenderPass).setWidth(swapchainExtent.width).setHeight(swapchainExtent.height);
			framebufferCreateInfo.setAttachmentCount(1).setPAttachments(imageView.get()).setLayers(1);
			auto newFramebuffer = new vk::Framebuffer(context->getDevice()->createFramebuffer(framebufferCreateInfo));

			destroyFramebuffer = [&](vk::Framebuffer* framebuffer)
			{
				if (context && context->getDevice())
				{
					context->getDevice()->destroyFramebuffer(*framebuffer);
				}
			};

			framebuffer = std::unique_ptr<vk::Framebuffer, decltype(destroyFramebuffer)>(newFramebuffer, destroyFramebuffer);
		}

		void SwapchainImage::createCommandBuffer()
		{
			const auto commandBufferAllocateInfo = vk::CommandBufferAllocateInfo().setCommandPool(*context->getCommandPool()).setCommandBufferCount(1);
			commandBuffer = context->getDevice()->allocateCommandBuffers(commandBufferAllocateInfo).at(0);
		}

		SwapchainImage::SwapchainImage(const Context* _context, vk::Format _swapchainSurfaceFormat, vk::Extent2D _swapchainExtent, const vk::RenderPass* _swapchainRenderPass, const vk::Image& _image)
			: context(_context), swapchainSurfaceFormat(_swapchainSurfaceFormat), swapchainExtent(_swapchainExtent), swapchainRenderPass(_swapchainRenderPass), image(_image)
		{
			createImageView();
			createFramebuffer();
			createCommandBuffer();
		}

		void SwapchainImage::recordCommandBuffer(const vk::Pipeline* pipeline) const
		{
			const auto commandBufferBeginInfo = vk::CommandBufferBeginInfo().setFlags(vk::CommandBufferUsageFlagBits::eSimultaneousUse);
			commandBuffer.begin(commandBufferBeginInfo);

			const std::array<float, 4> clearColor = { 0.0f, 0.0f, 0.0f, 1.0f };
			const std::array<vk::ClearValue, 1> clearValues = { vk::ClearColorValue(clearColor) };

			auto renderPassBeginInfo = vk::RenderPassBeginInfo().setRenderPass(*swapchainRenderPass);
			renderPassBeginInfo.setClearValueCount(static_cast<uint32_t>(clearValues.size())).setPClearValues(clearValues.data());
			renderPassBeginInfo.setRenderArea(vk::Rect2D(vk::Offset2D(), swapchainExtent)).setFramebuffer(*framebuffer.get());
			commandBuffer.beginRenderPass(renderPassBeginInfo, vk::SubpassContents::eInline);

			commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, *pipeline);
			commandBuffer.draw(3, 1, 0, 0);

			commandBuffer.endRenderPass();
			commandBuffer.end();
		}
	}
}