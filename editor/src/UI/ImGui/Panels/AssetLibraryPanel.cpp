#include "../../../../include/UI/ImGui/Panels/AssetLibraryPanel.h"
#include "../../../../../vendor/imgui/imgui.h"
#include <iostream>

GeneralDetailsPanel::GeneralDetailsPanel(ApplicationState& s) : state(s) {}

void GeneralDetailsPanel::Render() {
    SetPositionAndSize();

    ImGui::Begin("General Details");

    ImGui::Text("Frame Rate: %.1f FPS", ImGui::GetIO().Framerate);

    ImGui::End();
}
