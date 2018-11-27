#pragma once

#include "Context.hpp"

namespace asc
{
	namespace internal
	{
		class SwapchainImage
		{
		private:
			const Context* context;
			vk::Format swapchainSurfaceFormat;
			vk::Extent2D swapchainExtent;
			const vk::RenderPass* swapchainRenderPass;
			vk::Image image;
			vk::CommandBuffer commandBuffer;

			std::function<void(vk::ImageView*)> destroyImageView;
			std::unique_ptr<vk::ImageView, decltype(destroyImageView)> imageView;

			std::function<void(vk::Framebuffer*)> destroyFramebuffer;
			std::unique_ptr<vk::Framebuffer, decltype(destroyFramebuffer)> framebuffer;

			void createImageView();
			void createFramebuffer();
			void createCommandBuffer();

		public:
			SwapchainImage(const Context* _context, vk::Format _swapchainSurfaceFormat, vk::Extent2D _swapchainExtent, const vk::RenderPass* _swapchainRenderPass, const vk::Image& _image);

			void recordCommandBuffer(const vk::Pipeline* pipeline) const;

			vk::CommandBuffer getCommandBuffer() const { return commandBuffer; }
		};
	}
}
