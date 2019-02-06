#pragma once

#include <functional>
#include <vulkan/vulkan.hpp>

namespace asc
{
	namespace internal
	{
		class Frame
		{
		private:
			const vk::Device* device;

			std::function<void(vk::Semaphore*)> destroySemaphore;
			std::unique_ptr<vk::Semaphore, decltype(destroySemaphore)> imageAvailableSemaphore, renderFinishedSemaphore;

			std::function<void(vk::Fence*)> destroyFence;
			std::unique_ptr<vk::Fence, decltype(destroyFence)> fence;

			void createSemaphores();
			void createFence();

		public:
			Frame(const vk::Device* _device);

			vk::Semaphore* getImageAvailableSemaphore() const { return imageAvailableSemaphore.get(); }
			vk::Semaphore* getRenderFinishedSemaphore() const { return renderFinishedSemaphore.get(); }
			vk::Fence* getFence() const { return fence.get(); }
		};
	}
}