#include "../../include/Core/IndexBuffer.h"
#include "../../include/Core/Platform/OpenGL/OpenGLIndexBuffer.h"
#include "../../include/Core/RenderAPI.h"

std::shared_ptr<IndexBuffer> IndexBuffer::Create(const uint32* indices,
                                                 uint32 size) {
    switch (RenderAPI::Get()) {
        case GraphicsAPI::OpenGL:
            return std::make_shared<OpenGLIndexBuffer>(indices, size);
        // case GraphicsAPI::Vulkan:
        //     return std::make_shared<VulkanRenderContext>();
        default:
            return nullptr;
    }
}