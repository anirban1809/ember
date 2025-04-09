#include "../../include/Core/VertexBuffer.h"
#include "../../include/Core/Platform/OpenGL/OpenGLVertexBuffer.h"
#include "../../include/Core/RenderAPI.h"

std::shared_ptr<VertexBuffer> VertexBuffer::Create(const float* vertices,
                                                   uint32 size, uint32 stride) {
    switch (RenderAPI::Get()) {
        case GraphicsAPI::OpenGL:
            return std::make_shared<OpenGLVertexBuffer>(vertices, size, stride);
        // case GraphicsAPI::Vulkan:
        //     return std::make_shared<VulkanVertexBuffer>();
        default:
            return nullptr;
    }
}