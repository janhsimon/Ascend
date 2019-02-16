#pragma once

#include "SwapchainImage.hpp"

namespace asc
{
  namespace internal
  {
    class Swapchain
    {
    public:
      Swapchain(const Context* context);

      void recordImageCommandBuffers(const vk::Pipeline* pipeline, const VertexBuffer* vertexBuffer);

      vk::Extent2D getExtent() const { return extent; }
      vk::RenderPass* getRenderPass() const { return renderPass.get(); }
      vk::SwapchainKHR* getSwapchain() const { return swapchain.get(); }
      const SwapchainImage* getImage(uint32_t index) const { return &images.at(index); }

    private:
      const Context* context;
      vk::Format surfaceFormat;
      vk::ColorSpaceKHR surfaceColorSpace;
      vk::PresentModeKHR presentMode;
      uint32_t imageCount;
      vk::Extent2D extent;
      std::vector<SwapchainImage> images;

      std::function<void(vk::SwapchainKHR*)> destroySwapchain;
      std::unique_ptr<vk::SwapchainKHR, decltype(destroySwapchain)> swapchain;

      std::function<void(vk::RenderPass*)> destroyRenderPass;
      std::unique_ptr<vk::RenderPass, decltype(destroyRenderPass)> renderPass;

      void selectSurfaceFormat();
      void selectPresentMode();
      void selectSwapExtent();
      void createSwapchain();
      void createRenderPass();
      void createImages();
    };
  }
}