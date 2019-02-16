#include "Swapchain.hpp"

namespace asc
{
  namespace internal
  {
    Swapchain::Swapchain(const Context* context_) :
      context(context_)
    {
      selectSurfaceFormat();
      selectPresentMode();
      selectSwapExtent();
      createSwapchain();
      createRenderPass();
      createImages();
    }

    void Swapchain::recordImageCommandBuffers(const vk::Pipeline* pipeline, const VertexBuffer* vertexBuffer)
    {
      for (const auto& image : images)
      {
        image.recordCommandBuffer(pipeline, vertexBuffer);
      }
    }

    void Swapchain::selectSurfaceFormat()
    {
      constexpr auto WANTED_SURFACE_FORMAT = vk::Format::eB8G8R8A8Unorm;
      constexpr auto WANTED_SURFACE_COLOR_SPACE = vk::ColorSpaceKHR::eSrgbNonlinear;

      const auto availableSurfaceFormats = context->getPhysicalDevice().getSurfaceFormatsKHR(*context->getSurface());

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

      const auto availablePresentModes = context->getPhysicalDevice().getSurfacePresentModesKHR(*context->getSurface());
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
      const auto surfaceCapabilities = context->getPhysicalDevice().getSurfaceCapabilitiesKHR(*context->getSurface());
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

    void Swapchain::createRenderPass()
    {
      auto attachmentDescription = vk::AttachmentDescription().setFormat(surfaceFormat);
      attachmentDescription.setLoadOp(vk::AttachmentLoadOp::eClear).setStoreOp(vk::AttachmentStoreOp::eStore);
      attachmentDescription.setStencilLoadOp(vk::AttachmentLoadOp::eDontCare).setStencilStoreOp(vk::AttachmentStoreOp::eDontCare);
      attachmentDescription.setInitialLayout(vk::ImageLayout::eUndefined).setFinalLayout(vk::ImageLayout::ePresentSrcKHR);

      auto attachmentReference = vk::AttachmentReference().setAttachment(0).setLayout(vk::ImageLayout::eColorAttachmentOptimal);

      auto subpassDescription = vk::SubpassDescription().setPipelineBindPoint(vk::PipelineBindPoint::eGraphics);
      subpassDescription.setColorAttachmentCount(1).setPColorAttachments(&attachmentReference);

      auto subpassDependency = vk::SubpassDependency().setSrcSubpass(VK_SUBPASS_EXTERNAL).setDstSubpass(0);
      subpassDependency.setSrcStageMask(vk::PipelineStageFlagBits::eColorAttachmentOutput).setSrcAccessMask(vk::AccessFlags());
      subpassDependency.setDstStageMask(vk::PipelineStageFlagBits::eColorAttachmentOutput).setDstAccessMask(vk::AccessFlagBits::eColorAttachmentRead | vk::AccessFlagBits::eColorAttachmentWrite);

      auto renderPassCreateInfo = vk::RenderPassCreateInfo();
      renderPassCreateInfo.setAttachmentCount(1).setPAttachments(&attachmentDescription);
      renderPassCreateInfo.setSubpassCount(1).setPSubpasses(&subpassDescription);
      renderPassCreateInfo.setDependencyCount(1).setPDependencies(&subpassDependency);

      auto newRenderPass = new vk::RenderPass(context->getDevice()->createRenderPass(renderPassCreateInfo));

      destroyRenderPass = [&](vk::RenderPass* renderPass)
      {
        if (context && context->getDevice())
        {
          context->getDevice()->destroyRenderPass(*renderPass);
        }
      };

      renderPass = std::unique_ptr<vk::RenderPass, decltype(destroyRenderPass)>(newRenderPass, destroyRenderPass);
    }

    void Swapchain::createImages()
    {
      images.clear();

      const auto retrievedImages = context->getDevice()->getSwapchainImagesKHR(*swapchain);
      images.reserve(retrievedImages.size());

      for (const auto& image : retrievedImages)
      {
        images.emplace_back(context, surfaceFormat, extent, renderPass.get(), image);
      }
    }
  }
}