#pragma once

#include "Context.hpp"
#include "VertexBuffer.hpp"

namespace asc
{
  namespace internal
  {
    class SwapchainImage
    {
    public:
      SwapchainImage(const Context* context, vk::Format swapchainSurfaceFormat, vk::Extent2D swapchainExtent, const vk::RenderPass* swapchainRenderPass, const vk::Image& image);

      void recordCommandBuffer(const vk::Pipeline* pipeline, const VertexBuffer* vertexBuffer) const;

      vk::CommandBuffer getCommandBuffer() const { return commandBuffer; }

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
    };
  }
}
