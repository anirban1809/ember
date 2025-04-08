
#include "../../../include/UI/ImGui/ImGuiLayer.h"
#include "../../../include/UI/ApplicationState.h"
#include <cstdio>
#include <iostream>

ImGuiLayer::ImGuiLayer(ApplicationState& s) : state(s) {}

void ImGuiLayer::Init(void* windowHandle) {
    window = static_cast<GLFWwindow*>(windowHandle);
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    float xscale, yscale;
    glfwGetWindowContentScale(window, &xscale,
                              &yscale);  // xscale = 2.0 on Retina
    float scale = xscale;                // usually same for x and y

    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |=
        ImGuiConfigFlags_ViewportsEnable | ImGuiConfigFlags_DockingEnable;

    io.Fonts->Clear();  // clear previous fonts

    float fontSize = 10.0f * scale;

    ImFont* customFont = io.Fonts->AddFontFromFileTTF(
        "/Users/anirban/Downloads/Roboto-Regular.ttf", fontSize);
    io.Fonts->Build();
    if (customFont == nullptr) {
        fprintf(stderr, "Could not load custom font!\n");
    }

    int display_w, display_h;
    glfwGetFramebufferSize(window, &display_w, &display_h);
    int window_w, window_h;
    glfwGetWindowSize(window, &window_w, &window_h);

    float fb_scale_x = (float)display_w / window_w;
    float fb_scale_y = (float)display_h / window_h;

    io.DisplayFramebufferScale = ImVec2(fb_scale_x, fb_scale_y);

    io.FontDefault = customFont;
    ImGui::StyleColorsDark();

    // ImGuiStyle& style = ImGui::GetStyle();
    // ImVec4* colors = style.Colors;

    ImGuiStyle* style = &ImGui::GetStyle();
    ImVec4* colors = style->Colors;

    // Base colors for a pleasant and modern dark theme with dark accents
    colors[ImGuiCol_Text] =
        ImVec4(0.92f, 0.93f, 0.94f, 1.00f);  // Light grey text for readability
    colors[ImGuiCol_TextDisabled] =
        ImVec4(0.50f, 0.52f, 0.54f, 1.00f);  // Subtle grey for disabled text
    colors[ImGuiCol_WindowBg] = ImVec4(
        0.14f, 0.14f, 0.16f, 1.00f);  // Dark background with a hint of blue
    colors[ImGuiCol_ChildBg] = ImVec4(
        0.16f, 0.16f, 0.18f, 1.00f);  // Slightly lighter for child elements
    colors[ImGuiCol_PopupBg] =
        ImVec4(0.18f, 0.18f, 0.20f, 1.00f);  // Popup background
    colors[ImGuiCol_Border] =
        ImVec4(0.28f, 0.29f, 0.30f, 0.60f);  // Soft border color
    colors[ImGuiCol_BorderShadow] =
        ImVec4(0.00f, 0.00f, 0.00f, 0.00f);  // No border shadow
    colors[ImGuiCol_FrameBg] =
        ImVec4(0.20f, 0.22f, 0.24f, 1.00f);  // Frame background
    colors[ImGuiCol_FrameBgHovered] =
        ImVec4(0.22f, 0.24f, 0.26f, 1.00f);  // Frame hover effect
    colors[ImGuiCol_FrameBgActive] =
        ImVec4(0.24f, 0.26f, 0.28f, 1.00f);  // Active frame background
    colors[ImGuiCol_TitleBg] =
        ImVec4(0.14f, 0.14f, 0.16f, 1.00f);  // Title background
    colors[ImGuiCol_TitleBgActive] =
        ImVec4(0.16f, 0.16f, 0.18f, 1.00f);  // Active title background
    colors[ImGuiCol_TitleBgCollapsed] =
        ImVec4(0.14f, 0.14f, 0.16f, 1.00f);  // Collapsed title background
    colors[ImGuiCol_MenuBarBg] =
        ImVec4(0.20f, 0.20f, 0.22f, 1.00f);  // Menu bar background
    colors[ImGuiCol_ScrollbarBg] =
        ImVec4(0.16f, 0.16f, 0.18f, 1.00f);  // Scrollbar background
    colors[ImGuiCol_ScrollbarGrab] =
        ImVec4(0.24f, 0.26f, 0.28f, 1.00f);  // Dark accent for scrollbar grab
    colors[ImGuiCol_ScrollbarGrabHovered] =
        ImVec4(0.28f, 0.30f, 0.32f, 1.00f);  // Scrollbar grab hover
    colors[ImGuiCol_ScrollbarGrabActive] =
        ImVec4(0.32f, 0.34f, 0.36f, 1.00f);  // Scrollbar grab active
    colors[ImGuiCol_CheckMark] =
        ImVec4(0.46f, 0.56f, 0.66f, 1.00f);  // Dark blue checkmark
    colors[ImGuiCol_SliderGrab] =
        ImVec4(0.36f, 0.46f, 0.56f, 1.00f);  // Dark blue slider grab
    colors[ImGuiCol_SliderGrabActive] =
        ImVec4(0.40f, 0.50f, 0.60f, 1.00f);  // Active slider grab
    colors[ImGuiCol_Button] =
        ImVec4(0.24f, 0.34f, 0.44f, 1.00f);  // Dark blue button
    colors[ImGuiCol_ButtonHovered] =
        ImVec4(0.28f, 0.38f, 0.48f, 1.00f);  // Button hover effect
    colors[ImGuiCol_ButtonActive] =
        ImVec4(0.32f, 0.42f, 0.52f, 1.00f);  // Active button
    colors[ImGuiCol_Header] =
        ImVec4(0.24f, 0.34f, 0.44f, 1.00f);  // Header color similar to button
    colors[ImGuiCol_HeaderHovered] =
        ImVec4(0.28f, 0.38f, 0.48f, 1.00f);  // Header hover effect
    colors[ImGuiCol_HeaderActive] =
        ImVec4(0.32f, 0.42f, 0.52f, 1.00f);  // Active header
    colors[ImGuiCol_Separator] =
        ImVec4(0.28f, 0.29f, 0.30f, 1.00f);  // Separator color
    colors[ImGuiCol_SeparatorHovered] =
        ImVec4(0.46f, 0.56f, 0.66f, 1.00f);  // Hover effect for separator
    colors[ImGuiCol_SeparatorActive] =
        ImVec4(0.46f, 0.56f, 0.66f, 1.00f);  // Active separator
    colors[ImGuiCol_ResizeGrip] =
        ImVec4(0.36f, 0.46f, 0.56f, 1.00f);  // Resize grip
    colors[ImGuiCol_ResizeGripHovered] =
        ImVec4(0.40f, 0.50f, 0.60f, 1.00f);  // Hover effect for resize grip
    colors[ImGuiCol_ResizeGripActive] =
        ImVec4(0.44f, 0.54f, 0.64f, 1.00f);  // Active resize grip
    colors[ImGuiCol_Tab] = ImVec4(0.20f, 0.22f, 0.24f, 1.00f);  // Inactive tab
    colors[ImGuiCol_TabHovered] =
        ImVec4(0.28f, 0.38f, 0.48f, 1.00f);  // Hover effect for tab
    colors[ImGuiCol_TabActive] =
        ImVec4(0.24f, 0.34f, 0.44f, 1.00f);  // Active tab color
    colors[ImGuiCol_TabUnfocused] =
        ImVec4(0.20f, 0.22f, 0.24f, 1.00f);  // Unfocused tab
    colors[ImGuiCol_TabUnfocusedActive] =
        ImVec4(0.24f, 0.34f, 0.44f, 1.00f);  // Active but unfocused tab
    colors[ImGuiCol_PlotLines] =
        ImVec4(0.46f, 0.56f, 0.66f, 1.00f);  // Plot lines
    colors[ImGuiCol_PlotLinesHovered] =
        ImVec4(0.46f, 0.56f, 0.66f, 1.00f);  // Hover effect for plot lines
    colors[ImGuiCol_PlotHistogram] =
        ImVec4(0.36f, 0.46f, 0.56f, 1.00f);  // Histogram color
    colors[ImGuiCol_PlotHistogramHovered] =
        ImVec4(0.40f, 0.50f, 0.60f, 1.00f);  // Hover effect for histogram
    colors[ImGuiCol_TableHeaderBg] =
        ImVec4(0.20f, 0.22f, 0.24f, 1.00f);  // Table header background
    colors[ImGuiCol_TableBorderStrong] =
        ImVec4(0.28f, 0.29f, 0.30f, 1.00f);  // Strong border for tables
    colors[ImGuiCol_TableBorderLight] =
        ImVec4(0.24f, 0.25f, 0.26f, 1.00f);  // Light border for tables
    colors[ImGuiCol_TableRowBg] =
        ImVec4(0.20f, 0.22f, 0.24f, 1.00f);  // Table row background
    colors[ImGuiCol_TableRowBgAlt] =
        ImVec4(0.22f, 0.24f, 0.26f, 1.00f);  // Alternate row background
    colors[ImGuiCol_TextSelectedBg] =
        ImVec4(0.24f, 0.34f, 0.44f, 0.35f);  // Selected text background
    colors[ImGuiCol_DragDropTarget] =
        ImVec4(0.46f, 0.56f, 0.66f, 0.90f);  // Drag and drop target
    colors[ImGuiCol_NavHighlight] =
        ImVec4(0.46f, 0.56f, 0.66f, 1.00f);  // Navigation highlight
    colors[ImGuiCol_NavWindowingHighlight] =
        ImVec4(1.00f, 1.00f, 1.00f, 0.70f);  // Windowing highlight
    colors[ImGuiCol_NavWindowingDimBg] =
        ImVec4(0.80f, 0.80f, 0.80f, 0.20f);  // Dim background for windowing
    colors[ImGuiCol_ModalWindowDimBg] =
        ImVec4(0.80f, 0.80f, 0.80f, 0.35f);  // Dim background for modal windows

    // Style adjustments
    style->WindowPadding = ImVec2(8.00f, 8.00f);
    style->FramePadding = ImVec2(5.00f, 2.00f);
    style->CellPadding = ImVec2(6.00f, 6.00f);
    style->ItemSpacing = ImVec2(6.00f, 6.00f);
    style->ItemInnerSpacing = ImVec2(6.00f, 6.00f);
    style->TouchExtraPadding = ImVec2(0.00f, 0.00f);
    style->IndentSpacing = 25;
    style->ScrollbarSize = 11;
    style->GrabMinSize = 10;
    style->WindowBorderSize = 1;
    style->ChildBorderSize = 1;
    style->PopupBorderSize = 1;
    style->FrameBorderSize = 1;
    style->TabBorderSize = 1;
    style->WindowRounding = 7;
    style->ChildRounding = 4;
    style->FrameRounding = 3;
    style->PopupRounding = 4;
    style->ScrollbarRounding = 9;
    style->GrabRounding = 3;
    style->LogSliderDeadzone = 4;
    style->TabRounding = 4;

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glslVersion);
}

ImGuiIO ImGuiLayer::GetIO() { return ImGui::GetIO(); }

void ImGuiLayer::BeginFrame() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("Scene")) {
            if (ImGui::MenuItem("New")) {
                std::cout << "Create New Project" << std::endl;
            }
            if (ImGui::MenuItem("Open")) { /* action */
            }

            if (ImGui::MenuItem("Load Object")) { /* action */
                std::cout << "Load object" << std::endl;
                state.openLoadObjectPanel = true;
            }

            ImGui::Separator();

            if (ImGui::MenuItem("Save")) { /* action */
            }

            if (ImGui::MenuItem("Save As")) { /* action */
            }

            if (ImGui::MenuItem("Exit")) { /* action */
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Edit")) {
            if (ImGui::MenuItem("Undo", "CTRL+Z")) { /* action */
                std::cout << "State = " << state.stateValue << std::endl;
            }
            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }
}

void ImGuiLayer::EndFrame() {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        GLFWwindow* backup = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup);
    }
}

void ImGuiLayer::Shutdown() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}