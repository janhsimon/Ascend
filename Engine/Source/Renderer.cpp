#include "Renderer.hpp"

namespace asc
{
	Renderer::Renderer(const Application& application) :
		currentFrameIndex(0)
	{
		const auto cppStyleInstance = reinterpret_cast<vk::Instance*>(application.getInstance());
		context = std::make_unique<internal::Context>(cppStyleInstance, application.getApplicationInfo());

		frames.resize(MAX_FRAMES_IN_PIPELINE);
		for (size_t i = 0; i < frames.size(); ++i)
		{
			frames[i] = std::make_unique<internal::Frame>(context->getDevice());
		}
	
		swapchain = std::make_unique<internal::Swapchain>(context.get());
		pipeline = std::make_unique<internal::Pipeline>(context->getDevice(), swapchain->getExtent(), swapchain->getRenderPass());
		
		swapchain->recordImageCommandBuffers(pipeline->getPipeline());
	}

	Renderer::~Renderer()
	{
		context->getDevice()->waitIdle();
	}

	/*
	void Renderer::loadShader(const std::string& filename, const ShaderType type)
	{
		const auto flags = (type == ShaderType::Vertex) ? vk::ShaderStageFlagBits::eVertex : vk::ShaderStageFlagBits::eFragment;
		internal::Shader vertexShader(context->getDevice(), filename, flags);
	}
	*/

	void Renderer::renderFrame()
	{	
		const auto frame = frames[currentFrameIndex].get();
		context->getDevice()->waitForFences({ *frame->getFence() }, true, std::numeric_limits<uint64_t>::max());
		context->getDevice()->resetFences({ *frame->getFence() });

		const auto nextImage = context->getDevice()->acquireNextImageKHR(*swapchain->getSwapchain(), std::numeric_limits<uint64_t>::max(), *frame->getImageAvailableSemaphore(), nullptr);
		const auto imageIndex = nextImage.value;

		{
			const std::array<vk::Semaphore, 1> waitForSemaphores = { *frame->getImageAvailableSemaphore() };
			const std::array<vk::PipelineStageFlags, 1> waitForStages = { vk::PipelineStageFlagBits::eColorAttachmentOutput };
			const std::array<vk::Semaphore, 1> signalSemaphores = { *frame->getRenderFinishedSemaphore() };
			const std::array<vk::CommandBuffer, 1> commandBuffers = { swapchain->getImage(imageIndex)->getCommandBuffer() };

			auto submitInfo = vk::SubmitInfo();
			submitInfo.setWaitSemaphoreCount(static_cast<uint32_t>(waitForSemaphores.size())).setPWaitSemaphores(waitForSemaphores.data()).setPWaitDstStageMask(waitForStages.data());
			submitInfo.setSignalSemaphoreCount(static_cast<uint32_t>(signalSemaphores.size())).setPSignalSemaphores(signalSemaphores.data());
			submitInfo.setCommandBufferCount(static_cast<uint32_t>(commandBuffers.size())).setPCommandBuffers(commandBuffers.data());
			context->getGraphicsQueue().submit({ submitInfo }, *frame->getFence());
		}

		{
			const std::array<vk::Semaphore, 1> waitForSemaphores = { *frame->getRenderFinishedSemaphore() };
			const std::array<vk::SwapchainKHR, 1> swapchains = { *swapchain->getSwapchain() };

			auto presentInfo = vk::PresentInfoKHR().setWaitSemaphoreCount(static_cast<uint32_t>(waitForSemaphores.size())).setPWaitSemaphores(waitForSemaphores.data());
			presentInfo.setSwapchainCount(static_cast<uint32_t>(swapchains.size())).setPSwapchains(swapchains.data()).setPImageIndices(&imageIndex);
			context->getPresentQueue().presentKHR(&presentInfo);
		}

		currentFrameIndex = (++currentFrameIndex) % frames.size();
	}
}