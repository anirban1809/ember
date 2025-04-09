#include "../../include/Core/VertexArray.h"
#include "../../include/Core/Platform/OpenGL/OpenGLVertexArray.h"
#include "../../include/Core/RenderAPI.h"

std::shared_ptr<VertexArray> VertexArray::Create() {
    switch (RenderAPI::Get()) {
        case GraphicsAPI::OpenGL:
            return std::make_shared<OpenGLVertexArray>();
        // case GraphicsAPI::Vulkan:
        //     return std::make_shared<VulkanVertexArray>();
        default:
            return nullptr;
    }
}