
#include "EmberEditor.h"
#include "Core/Events.h"
#include "Core/Logger.h"
#include "Core/Material.h"
#include "Core/ShaderProgram.h"
#include "Core/Types.h"
#include "Core/VertexLayout.h"
#include "UI/ImGui/ImGuiLayer.h"
#include "UI/ImGui/ImGuiLayoutContainer.h"
#include "UI/ImGui/Panels/AssetLibraryPanel.h"
#include "UI/ImGui/Panels/FrameBufferPanel.h"
#include "UI/ImGui/Panels/LoadObjectPanel.h"
#include "UI/ImGui/Panels/NodePropsPanel.h"
#include "UI/ImGui/Panels/ScenePropsPanel.h"
#include "glm/ext/quaternion_common.hpp"
#include "importer.h"
#include <memory>
EmberEditor::EmberEditor(int width, int height, const char* title)
    : Application(width, height, title),
      state(fs, assets),
      uiEngine(std::make_unique<ImGuiLayer>(state)) {}

void EmberEditor::OnInit() {
    Logger::Log(LOG_INFO, "Initializing Ember Engine 0.0.1");
    context = RenderContext::Create();
    scenebuffer = FrameBuffer::Create(1000.0f, 1000.0f);
    uiEngine.Init(window->GetGLFWWindow());
    DefineUI();

    std::vector<float> model_vertices;

    std::vector<uint32> model_indices;

    if (Importer::Load("/Users/anirban/Documents/Code/engine/editor/models/"
                       "testscene.obj",
                       model_vertices, model_indices)) {
        std::cout << "Loaded mesh!" << std::endl;
        std::cout << "Unique vertices: " << model_vertices.size() / 8
                  << std::endl;
        std::cout << "Indices: " << model_indices.size() << std::endl;
    } else {
        std::cerr << "Failed to load mesh." << std::endl;
    }

    auto mainshader = ShaderProgram::Create(
        "/Users/anirban/Documents/Code/engine/editor/Shaders/"
        "vertex_shader.glsl",
        "/Users/anirban/Documents/Code/engine/editor/Shaders/"
        "fragment_shader.glsl");

    m_GridShader = ShaderProgram::Create(
        "/Users/anirban/Documents/Code/engine/editor/Shaders/grid/"
        "grid_vertex_shader.glsl",
        "/Users/anirban/Documents/Code/engine/editor/Shaders/grid/"
        "grid_fragment_shader.glsl");

    // Store shader in scene
    shaders.push_back(mainshader);

    // Set up material
    auto material = std::make_shared<Material>(mainshader);

    // Set up camera
    camera.SetCameraProjection(45.0f, 1.0f, 0.1f, 1000.0f);
    camera.SetCameraPosition(0.0f, 100.0f, 100.0f);
    camera.SetCameraLook(0.0f, 0.0f, 0.0f);

    // Set up light
    light.SetColor(glm::vec3(1.0f));
    light.SetPosition(glm::vec3(0.0, 10.0, 0.0));

    // Create mesh
    meshes.emplace_back(
        model_vertices, model_indices,
        VertexLayout{
            {ShaderType::Float3, 0 * sizeof(float), 0},  // position
            {ShaderType::Float2, 3 * sizeof(float), 1},  // texcoord
            {ShaderType::Float3, 5 * sizeof(float), 2},  // normal
        },
        glm::mat4(1.0f), material);
}

void EmberEditor::OnUpdate() {}

void EmberEditor::OnRender() {
    // Render ImGUI Panels
    uiEngine.BeginFrame();
    uiEngine.RenderPanels();
    uiEngine.EndFrame();

    // Set global uniforms on all shaders
    for (auto& shader : shaders) {
        shader->Bind();
        shader->SetUniformFloat3("diffuseColor",
                                 glm::vec3(1.0f, 1.0f, 1.0f));  // example color
        shader->SetUniformMat4("view", camera.GetView());
        shader->SetUniformMat4("projection", camera.GetProjection());
        shader->SetUniformFloat3("viewPos", camera.GetCameraPosition());
        shader->SetUniformFloat3("lightPos", light.GetPosition());
        shader->SetUniformFloat3("lightColor", light.GetColor());
    }

    scenebuffer->Bind();
    context->Clear();
    context->BeginScene(shaders);
    m_GridRenderer.Render(context, m_GridShader, camera);
    for (auto& mesh : meshes) {
        context->Submit(mesh);
    }
    context->EndScene();

    scenebuffer->Unbind();
}

//----------------------------UI Definition----------------------------//

void EmberEditor::DefineUI() {
    std::shared_ptr<ImGuiLayoutContainer> lc =
        CreateLayoutContainer<ImGuiLayoutContainer>(1, 14);

    lc->SetGap(10.0f);

    std::shared_ptr<ImGuiLayoutContainer> lc1 =
        CreateLayoutContainer<ImGuiLayoutContainer>(2, 1);
    std::shared_ptr<ImGuiLayoutContainer> lc2 =
        CreateLayoutContainer<ImGuiLayoutContainer>(1, 1);
    std::shared_ptr<ImGuiLayoutContainer> lc3 =
        CreateLayoutContainer<ImGuiLayoutContainer>(1, 1);

    lc1->AddElement(CreatePanel<ScenePropsPanel>(state), 1);
    lc1->AddElement(CreatePanel<AssetLibraryPanel>(state), 1);

    lc2->AddElement(CreatePanel<FramebufferPanel>("Scene", state, scenebuffer),
                    1);
    lc3->AddElement(CreatePanel<NodePropsPanel>(state), 1);

    lc->AddElement(lc1, 3);
    lc->AddElement(lc2, 9);
    lc->AddElement(lc3, 2);

    uiEngine.GetUIManager().AddLayoutContainer(lc);
    uiEngine.GetUIManager().AddPanel(CreatePanel<LoadObjectPanel>(state));
}

//-------------------------Input Handling--------------------------------//

void EmberEditor::OnKeyPressed(int key) {
    // Retrieve current camera position and look target.
    glm::vec3 position = camera.GetCameraPosition();
    glm::vec3 look = camera.GetCameraLook();

    // Calculate forward vector (direction the camera is facing)
    glm::vec3 forward = glm::normalize(look - position);
    // Compute right vector from forward and world up vector.
    glm::vec3 right =
        glm::normalize(glm::cross(forward, glm::vec3(0.0f, 1.0f, 0.0f)));

    if (key == KeyEvent::KEY_W) {
        std::cout << "Moving Forward" << std::endl;
        glm::vec3 delta = forward * movementSpeed;
        camera.SetCameraPosition(position.x + delta.x, position.y + delta.y,
                                 position.z + delta.z);
        camera.SetCameraLook(look.x + delta.x, look.y + delta.y,
                             look.z + delta.z);
    }
    if (key == KeyEvent::KEY_S) {
        glm::vec3 delta = forward * movementSpeed;
        camera.SetCameraPosition(position.x - delta.x, position.y - delta.y,
                                 position.z - delta.z);
        camera.SetCameraLook(look.x - delta.x, look.y - delta.y,
                             look.z - delta.z);
    }

    if (key == KeyEvent::KEY_A) {
        glm::vec3 delta = right * movementSpeed;
        camera.SetCameraPosition(position.x - delta.x, position.y - delta.y,
                                 position.z - delta.z);
        camera.SetCameraLook(look.x - delta.x, look.y - delta.y,
                             look.z - delta.z);
    }

    if (key == KeyEvent::KEY_D) {
        glm::vec3 delta = right * movementSpeed;
        camera.SetCameraPosition(position.x + delta.x, position.y + delta.y,
                                 position.z + delta.z);
        camera.SetCameraLook(look.x + delta.x, look.y + delta.y,
                             look.z + delta.z);
    }
}

void EmberEditor::OnMousePressed(int button) {
    if (!state.isFrameBufferPanelHovered) {
        return;
    }
    if (button == MouseEvent::MOUSE_BUTTON_LEFT) {
        std::cout << "Left mouse button clicked!" << std::endl;
        leftMouseDown = true;
        firstMouse = true;
    }
}

void EmberEditor::OnMouseReleased(int button) {
    if (!state.isFrameBufferPanelHovered) {
        return;
    }
    if (button == MouseEvent::MOUSE_BUTTON_LEFT) {
        std::cout << "Left mouse button released!" << std::endl;
        leftMouseDown = false;
    }
}

void EmberEditor::OnMouseMoved(double xpos, double ypos) {
    if (!leftMouseDown) {
        return;
    }

    if (firstMouse) {
        // On the first mouse event, initialize the last positions.
        lastX = (float)xpos;
        lastY = (float)ypos;
        firstMouse = false;
        return;
    }

    // Calculate the offset from the last frame.
    float xoffset = -(float)xpos + lastX;
    float yoffset = -lastY + (float)ypos;  // reversed: moving up is positive

    lastX = (float)xpos;
    lastY = (float)ypos;

    // Scale offsets by sensitivity.
    xoffset *= mouseSensitivity;
    yoffset *= mouseSensitivity;

    // Update yaw and pitch.
    yaw += xoffset;
    pitch += yoffset;

    // Constrain pitch to prevent flipping.
    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;

    // Compute the new front direction.
    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(front);

    // Update the camera look target.
    // (Camera look = camera position + front vector)
    glm::vec3 newLook = camera.GetCameraPosition() + front;
    camera.SetCameraLook(newLook.x, newLook.y, newLook.z);
}
