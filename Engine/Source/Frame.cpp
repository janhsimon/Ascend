#include "Frame.hpp"

namespace asc
{
  namespace internal
  {
    Frame::Frame(const Context* context_, const Swapchain* swapchain_) :
      context(context_),
      swapchain(swapchain_)
    {
      device = context->getDevice();

      createSemaphores();
      createFence();
    }

    void Frame::renderAndPresent() const
    {
      const auto nextSwapchainImageIndex = getNextSwapchainImageIndexWhenReady();
      render(nextSwapchainImageIndex);
      present(nextSwapchainImageIndex);
    }

    void Frame::createSemaphores()
    {
      const auto semaphoreCreateInfo = vk::SemaphoreCreateInfo();
      const auto newImageAvailableSemaphore = new vk::Semaphore(device->createSemaphore(semaphoreCreateInfo));
      const auto newRenderFinishedSemaphore = new vk::Semaphore(device->createSemaphore(semaphoreCreateInfo));

      destroySemaphore = [&](vk::Semaphore* semaphore)
      {
        if (device)
        {
          device->destroySemaphore(*semaphore);
        }
      };

      imageAvailableSemaphore = std::unique_ptr<vk::Semaphore, decltype(destroySemaphore)>(newImageAvailableSemaphore, destroySemaphore);
      renderFinishedSemaphore = std::unique_ptr<vk::Semaphore, decltype(destroySemaphore)>(newRenderFinishedSemaphore, destroySemaphore);
    }

    void Frame::createFence()
    {
      const auto fenceCreateInfo = vk::FenceCreateInfo().setFlags(vk::FenceCreateFlagBits::eSignaled);
      const auto newFence = new vk::Fence(device->createFence(fenceCreateInfo));

      destroyFence = [&](vk::Fence* fence)
      {
        if (device)
        {
          device->destroyFence(*fence);
        }
      };

      fence = std::unique_ptr<vk::Fence, decltype(destroyFence)>(newFence, destroyFence);
    }

    uint32_t Frame::getNextSwapchainImageIndexWhenReady() const
    {
      device->waitForFences({ *fence.get() }, true, std::numeric_limits<uint64_t>::max());
      device->resetFences({ *fence.get() });
      return context->getDevice()->acquireNextImageKHR(*swapchain->getSwapchain(), std::numeric_limits<uint64_t>::max(), *imageAvailableSemaphore.get(), nullptr).value;
    }

    void Frame::render(const uint32_t swapchainImageIndex) const
    {
      const std::array<vk::Semaphore, 1> waitForSemaphores = { *imageAvailableSemaphore.get() };
      const std::array<vk::PipelineStageFlags, 1> waitForStages = { vk::PipelineStageFlagBits::eColorAttachmentOutput };
      const std::array<vk::Semaphore, 1> signalSemaphores = { *renderFinishedSemaphore.get() };
      const std::array<vk::CommandBuffer, 1> commandBuffers = { swapchain->getImage(swapchainImageIndex)->getCommandBuffer() };

      auto submitInfo = vk::SubmitInfo();
      submitInfo.setWaitSemaphoreCount(static_cast<uint32_t>(waitForSemaphores.size())).setPWaitSemaphores(waitForSemaphores.data()).setPWaitDstStageMask(waitForStages.data());
      submitInfo.setSignalSemaphoreCount(static_cast<uint32_t>(signalSemaphores.size())).setPSignalSemaphores(signalSemaphores.data());
      submitInfo.setCommandBufferCount(static_cast<uint32_t>(commandBuffers.size())).setPCommandBuffers(commandBuffers.data());
      context->getGraphicsQueue().submit({ submitInfo }, *fence.get());
    }

    void Frame::present(const uint32_t swapchainImageIndex) const
    {
      const std::array<vk::Semaphore, 1> waitForSemaphores = { *renderFinishedSemaphore.get() };
      const std::array<vk::SwapchainKHR, 1> swapchains = { *swapchain->getSwapchain() };

      auto presentInfo = vk::PresentInfoKHR().setWaitSemaphoreCount(static_cast<uint32_t>(waitForSemaphores.size())).setPWaitSemaphores(waitForSemaphores.data());
      presentInfo.setSwapchainCount(static_cast<uint32_t>(swapchains.size())).setPSwapchains(swapchains.data()).setPImageIndices(&swapchainImageIndex);
      context->getPresentQueue().presentKHR(&presentInfo);
    }
  }
}