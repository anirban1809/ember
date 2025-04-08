#include "../../include/Core/RenderContext.h"
#include "../../include/Core/Platform/OpenGL/OpenGLRenderContext.h"
#include "../../include/Core/RenderAPI.h"

std::shared_ptr<RenderContext> RenderContext::Create() {
    switch (RenderAPI::Get()) {
        case GraphicsAPI::OpenGL:
            return std::make_shared<OpenGLRenderContext>();
        // case GraphicsAPI::Vulkan:
        //     return std::make_shared<VulkanRenderContext>();
        default:
            return nullptr;
    }
}