#pragma once

#include "Swapchain.hpp"

namespace asc
{
  namespace internal
  {
    class Frame
    {
    public:
      Frame(const Context* context, const Swapchain* swapchain);

      void renderAndPresent() const;

    private:
      const Context* context;
      const vk::Device* device;
      const Swapchain* swapchain;

      std::function<void(vk::Semaphore*)> destroySemaphore;
      std::unique_ptr<vk::Semaphore, decltype(destroySemaphore)> imageAvailableSemaphore, renderFinishedSemaphore;

      std::function<void(vk::Fence*)> destroyFence;
      std::unique_ptr<vk::Fence, decltype(destroyFence)> fence;

      void createSemaphores();
      void createFence();

      uint32_t getNextSwapchainImageIndexWhenReady() const;
      void render(const uint32_t swapchainImageIndex) const;
      void present(const uint32_t swapchainImageIndex) const;
    };
  }
}