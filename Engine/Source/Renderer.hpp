#pragma once

#include "Frame.hpp"
#include "Pipeline.hpp"
#include "Swapchain.hpp"

namespace asc
{
  /*
  enum ShaderType
  {
    Vertex,
    Fragment
  };
  */

  class Renderer
  {
  public:
    Renderer(const Application& application);
    ~Renderer();

    //void loadShader(const std::string& filename, const ShaderType type);
    void render();

  private:
    static constexpr uint16_t MAX_FRAMES_IN_PIPELINE = 2;

    std::unique_ptr<internal::Context> context;
    std::vector<std::unique_ptr<internal::Frame>> frames;
    uint16_t currentFrameIndex;
    std::unique_ptr<internal::Swapchain> swapchain;
    std::unique_ptr<internal::Pipeline> pipeline;
    std::unique_ptr<internal::VertexBuffer> vertexBuffer;
  };
}