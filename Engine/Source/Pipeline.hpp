#pragma once

#include "Context.hpp"

namespace asc
{
  namespace internal
  {
    class Pipeline
    {
    public:
      Pipeline(const vk::Device* device, const vk::Extent2D swapchainExtent, const vk::RenderPass* swapchainRenderPass);

      vk::Pipeline* getPipeline() const { return pipeline.get(); };

    private:
      const vk::Device* device;
      vk::Extent2D swapchainExtent;
      const vk::RenderPass* swapchainRenderPass;

      std::function<void(vk::PipelineLayout*)> destroyPipelineLayout;
      std::unique_ptr<vk::PipelineLayout, decltype(destroyPipelineLayout)> pipelineLayout;

      std::function<void(vk::Pipeline*)> destroyPipeline;
      std::unique_ptr<vk::Pipeline, decltype(destroyPipeline)> pipeline;

      void createPipelineLayout();
      void createPipeline();
    };
  }
}