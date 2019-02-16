#include "Pipeline.hpp"
#include "Shader.hpp"
#include "VertexBuffer.hpp"

namespace asc
{
  namespace internal
  {
    Pipeline::Pipeline(const vk::Device* device_, const vk::Extent2D swapchainExtent_, const vk::RenderPass* swapchainRenderPass_) :
      device(device_), swapchainExtent(swapchainExtent_), swapchainRenderPass(swapchainRenderPass_)
    {
      createPipelineLayout();
      createPipeline();
    }

    void Pipeline::createPipelineLayout()
    {
      auto pipelineLayoutCreateInfo = vk::PipelineLayoutCreateInfo();
      auto newPipelineLayout = new vk::PipelineLayout(device->createPipelineLayout(pipelineLayoutCreateInfo));

      destroyPipelineLayout = [&](vk::PipelineLayout* pipelineLayout)
      {
        if (device)
        {
          device->destroyPipelineLayout(*pipelineLayout);
        }
      };

      pipelineLayout = std::unique_ptr<vk::PipelineLayout, decltype(destroyPipelineLayout)>(newPipelineLayout, destroyPipelineLayout);
    }

    void Pipeline::createPipeline()
    {
      const Shader vertexShader(device, "Shaders/Test.vert.spv", vk::ShaderStageFlagBits::eVertex);
      const Shader fragmentShader(device, "Shaders/Test.frag.spv", vk::ShaderStageFlagBits::eFragment);
      const std::array<vk::PipelineShaderStageCreateInfo, 2> pipelineShaderStageCreateInfos =
      {
        vertexShader.getPipelineShaderStageCreateInfo(),
        fragmentShader.getPipelineShaderStageCreateInfo()
      };

      auto vertexInputBindingDescription = vk::VertexInputBindingDescription().setStride(sizeof(Vertex));
      auto vertexPosition = vk::VertexInputAttributeDescription().setLocation(0).setFormat(vk::Format::eR32G32Sfloat).setOffset(offsetof(Vertex, position));
      auto vertexColor = vk::VertexInputAttributeDescription().setLocation(1).setFormat(vk::Format::eR32G32B32Sfloat).setOffset(offsetof(Vertex, color));
      std::vector<vk::VertexInputAttributeDescription> vertexInputAttributeDescriptions = { vertexPosition, vertexColor };
      auto vertexInputStateCreateInfo = vk::PipelineVertexInputStateCreateInfo().setVertexBindingDescriptionCount(1).setPVertexBindingDescriptions(&vertexInputBindingDescription);
      vertexInputStateCreateInfo.setVertexAttributeDescriptionCount(static_cast<uint32_t>(vertexInputAttributeDescriptions.size()));
      vertexInputStateCreateInfo.setPVertexAttributeDescriptions(vertexInputAttributeDescriptions.data());

      auto inputAssemblyStateCreateInfo = vk::PipelineInputAssemblyStateCreateInfo().setTopology(vk::PrimitiveTopology::eTriangleList);

      auto viewport = vk::Viewport().setWidth(static_cast<float>(swapchainExtent.width)).setHeight(static_cast<float>(swapchainExtent.height)).setMaxDepth(1.0f);
      auto scissor = vk::Rect2D().setExtent(swapchainExtent);
      auto viewportStateCreateInfo = vk::PipelineViewportStateCreateInfo().setViewportCount(1).setPViewports(&viewport).setScissorCount(1).setPScissors(&scissor);

      auto rasterizationStateCreateInfo = vk::PipelineRasterizationStateCreateInfo().setCullMode(vk::CullModeFlagBits::eBack).setFrontFace(vk::FrontFace::eClockwise).setLineWidth(1.0f);

      auto multisampleStateCreateInfo = vk::PipelineMultisampleStateCreateInfo().setMinSampleShading(1.0f);

      auto depthStenctilStateCreateInfo = vk::PipelineDepthStencilStateCreateInfo();

      auto colorBlendAttachmentState = vk::PipelineColorBlendAttachmentState().setSrcColorBlendFactor(vk::BlendFactor::eOne).setSrcAlphaBlendFactor(vk::BlendFactor::eOne);
      colorBlendAttachmentState.setColorWriteMask(vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA);
      auto colorBlendStateCreateInfo = vk::PipelineColorBlendStateCreateInfo().setAttachmentCount(1).setPAttachments(&colorBlendAttachmentState);

      auto pipelineCreateInfo = vk::GraphicsPipelineCreateInfo().setStageCount(static_cast<uint32_t>(pipelineShaderStageCreateInfos.size())).setPStages(pipelineShaderStageCreateInfos.data());
      pipelineCreateInfo.setPVertexInputState(&vertexInputStateCreateInfo).setPInputAssemblyState(&inputAssemblyStateCreateInfo).setPViewportState(&viewportStateCreateInfo);
      pipelineCreateInfo.setPRasterizationState(&rasterizationStateCreateInfo).setPMultisampleState(&multisampleStateCreateInfo).setPDepthStencilState(&depthStenctilStateCreateInfo);
      pipelineCreateInfo.setPColorBlendState(&colorBlendStateCreateInfo);
      pipelineCreateInfo.setRenderPass(*swapchainRenderPass).setLayout(*pipelineLayout);
      auto newPipeline = new vk::Pipeline(device->createGraphicsPipeline(nullptr, pipelineCreateInfo));
			
      destroyPipeline = [&](vk::Pipeline* pipeline)
      {
        if (device)
        {
          device->destroyPipeline(*pipeline);
        }
      };

      pipeline = std::unique_ptr<vk::Pipeline, decltype(destroyPipeline)>(newPipeline, destroyPipeline);
    }
  }
}