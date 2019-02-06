#include "Frame.hpp"

namespace asc
{
	namespace internal
	{
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

		Frame::Frame(const vk::Device* _device) :
			device(_device)
		{
			createSemaphores();
			createFence();
		}
	}
}