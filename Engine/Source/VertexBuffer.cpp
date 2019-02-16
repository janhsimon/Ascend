#include "Log.hpp"
#include "VertexBuffer.hpp"

namespace asc
{
  namespace internal
  {
    VertexBuffer::VertexBuffer(const Context* context_) :
      context(context_)
    {
      vertices =
      {
        {{0.0f, -0.5f}, {1.0f, 1.0f, 1.0f}},
        {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
        {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}
      };

      const auto bufferCreateInfo = vk::BufferCreateInfo().setUsage(vk::BufferUsageFlagBits::eVertexBuffer).setSize(sizeof(vertices[0]) * vertices.size());
      const auto newBuffer = new vk::Buffer(context->getDevice()->createBuffer(bufferCreateInfo));

      destroyBuffer = [&](vk::Buffer* buffer)
      {
        if (context && context->getDevice())
        {
          context->getDevice()->destroyBuffer(*buffer);
        }
      };

      buffer = std::unique_ptr<vk::Buffer, decltype(destroyBuffer)>(newBuffer, destroyBuffer);

      const auto memoryRequirements = context->getDevice()->getBufferMemoryRequirements(*buffer.get());
      const auto memoryProperties = context->getPhysicalDevice().getMemoryProperties();

      const auto wantedProperties = vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent;

      uint32_t memoryTypeIndex = 0;
      bool foundMatch = false;
      for (uint32_t i = 0; i < memoryProperties.memoryTypeCount; ++i)
      {
        if ((memoryRequirements.memoryTypeBits & (1 << i)) && (memoryProperties.memoryTypes[i].propertyFlags & wantedProperties) == wantedProperties)
        {
          memoryTypeIndex = i;
          foundMatch = true;
          break;
        }
      }

      if (!foundMatch)
      {
        Log("Failed to find suitable memory type for vertex buffer.", LogSeverity::Fatal);
      }

      const auto memoryAllocateInfo = vk::MemoryAllocateInfo().setAllocationSize(memoryRequirements.size).setMemoryTypeIndex(memoryTypeIndex);
      const auto newDeviceMemory = new vk::DeviceMemory(context->getDevice()->allocateMemory(memoryAllocateInfo));

      destroyDeviceMemory = [&](vk::DeviceMemory* deviceMemory)
      {
        if (context && context->getDevice())
        {
          context->getDevice()->freeMemory(*deviceMemory);
        }
      };

      deviceMemory = std::unique_ptr<vk::DeviceMemory, decltype(destroyDeviceMemory)>(newDeviceMemory, destroyDeviceMemory);

      context->getDevice()->bindBufferMemory(*buffer.get(), *deviceMemory.get(), 0);
			
      const auto data = context->getDevice()->mapMemory(*deviceMemory.get(), 0, VK_WHOLE_SIZE);
      memcpy(data, vertices.data(), sizeof(vertices[0]) * vertices.size());
      context->getDevice()->unmapMemory(*deviceMemory.get());
    }
  }
}