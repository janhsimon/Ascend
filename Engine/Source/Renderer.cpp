#include "Log.hpp"
#include "Renderer.hpp"

namespace asc
{
  Renderer::Renderer(const Application& application) :
    currentFrameIndex(0)
  {
    const auto cppStyleInstance = reinterpret_cast<vk::Instance*>(application.getInstance());
    context = std::make_unique<internal::Context>(cppStyleInstance, application.getApplicationInfo());
    swapchain = std::make_unique<internal::Swapchain>(context.get());

    frames.resize(MAX_FRAMES_IN_PIPELINE);
    for (size_t i = 0; i < frames.size(); ++i)
    {
      frames[i] = std::make_unique<internal::Frame>(context.get(), swapchain.get());
    }

    pipeline = std::make_unique<internal::Pipeline>(context->getDevice(), swapchain->getExtent(), swapchain->getRenderPass());
    vertexBuffer = std::make_unique<internal::VertexBuffer>(context.get());

    swapchain->recordImageCommandBuffers(pipeline->getPipeline(), vertexBuffer.get());
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

  void Renderer::render()
  {	
    const auto frame = frames.at(currentFrameIndex).get();
    frame->renderAndPresent();
    currentFrameIndex = (++currentFrameIndex) % frames.size();
  }
}