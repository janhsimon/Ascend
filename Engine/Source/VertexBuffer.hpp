#pragma once

#include "Context.hpp"
#include "glm/glm/glm.hpp"

namespace asc
{
  namespace internal
  {
    struct Vertex
    {
      glm::vec2 position;
      glm::vec3 color;
    };

    class VertexBuffer
    {
    public:
      VertexBuffer(const Context* context);

      size_t getNumVertices() const { return vertices.size(); }
      vk::Buffer* getBuffer() const { return buffer.get(); }

    private:
      const Context* context;

      std::vector<Vertex> vertices;

      std::function<void(vk::Buffer*)> destroyBuffer;
      std::unique_ptr<vk::Buffer, decltype(destroyBuffer)> buffer;

      std::function<void(vk::DeviceMemory*)> destroyDeviceMemory;
      std::unique_ptr<vk::DeviceMemory, decltype(destroyDeviceMemory)> deviceMemory;
    };
  }
}