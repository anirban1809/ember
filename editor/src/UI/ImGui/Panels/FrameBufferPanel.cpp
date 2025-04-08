#include "../../../../include/UI/ImGui/Panels/FrameBufferPanel.h"
#include "../../../../../vendor/imgui/imgui.h"
#include "../../../../../lib/include/Core/FrameBuffer.h"
#include "../../../../../vendor/glfw-3.4.bin.MACOS/include/GLFW/glfw3.h"
#include "../../../../include/UI/ApplicationState.h"
#include <iostream>

FramebufferPanel::FramebufferPanel(const std::string& title,
                                   ApplicationState& state,
                                   std::shared_ptr<FrameBuffer> sceneBuffer)
    : title(title), sceneBuffer(sceneBuffer), state(state) {}

void FramebufferPanel::SetTexture(unsigned int id, const glm::ivec2& size) {
    textureID = sceneBuffer->GetColorAttachmentTexture();
    textureSize = size;
}

void FramebufferPanel::Render() {
    SetPositionAndSize();
    ImGui::Begin(title.c_str(), nullptr);
    state.isFrameBufferPanelHovered = ImGui::IsWindowHovered();

    const float window_width = ImGui::GetContentRegionAvail().x;
    const float window_height = ImGui::GetContentRegionAvail().y;

    sceneBuffer->Resize(window_width, window_height);

    glViewport(0, 0, window_width, window_height);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    ImVec2 position = ImGui::GetCursorScreenPos();

    ImGui::Image(
        static_cast<ImTextureID>(
            static_cast<uintptr_t>(sceneBuffer->GetColorAttachmentTexture())),
        ImVec2(window_width, window_height), ImVec2(0, 1), ImVec2(1, 0));

    ImGui::End();
}

bool FramebufferPanel::IsHovered() const { return isHovered; }

glm::ivec2 FramebufferPanel::GetMouseFramebufferCoords() const {
    return mouseCoords;
}
