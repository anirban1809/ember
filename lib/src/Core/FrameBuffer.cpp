
#include "../../include/Core/FrameBuffer.h"
#include "../../include/Core/RenderAPI.h"
#include "../../include/Core/Platform/OpenGL/OpenGLFrameBuffer.h"

std::shared_ptr<FrameBuffer> FrameBuffer::Create(float width, float height) {
    switch (RenderAPI::Get()) {
        case GraphicsAPI::OpenGL:
            return std::make_shared<OpenGLFrameBuffer>(width, height);
        // case GraphicsAPI::Vulkan:
        //     return std::make_shared<VulkanFrameBuffer>(width, height);
        default:
            return nullptr;
    }
}
