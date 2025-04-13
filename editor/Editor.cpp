
#include "Editor.h"
#include "Core/Events.h"
#include "Core/Logger.h"
#include "Core/Material.h"
#include "Core/Mesh.h"
#include "Core/RenderContext.h"
#include "Core/ShaderProgram.h"
#include "Core/ShaderRegistry.h"
#include "Core/Types.h"
#include "Core/VertexLayout.h"
#include "ECS/Components/CameraComponent.h"
#include "ECS/Components/SkyboxComponent.h"
#include "ECS/Entity.h"
#include "ECS/Scene.h"
#include "ECS/Systems/SkyboxSystem.h"
#include "UI/ImGui/ImGuiLayer.h"
#include "UI/ImGui/ImGuiLayoutContainer.h"
#include "UI/ImGui/Panels/AssetLibraryPanel.h"
#include "UI/ImGui/Panels/FrameBufferPanel.h"
#include "UI/ImGui/Panels/LoadObjectPanel.h"
#include "UI/ImGui/Panels/NodePropsPanel.h"
#include "UI/ImGui/Panels/ScenePropsPanel.h"
#include "UI/TextureCube.h"
#include "glm/fwd.hpp"
#include "importer.h"
#include <memory>
Editor::Editor(int width, int height, const char* title)
    : Application(width, height, title),
      state(fs, assets),
      uiEngine(std::make_unique<ImGuiLayer>(state)),
      context(RenderContext::Create()),
      m_SkyboxSystem(context),
      m_GridSystem(context) {}

void Editor::AddMesh(Mesh& mesh) { meshes.push_back(mesh); }

void Editor::OnInit() {
    Logger::Log(LOG_INFO, "Initializing Ember Engine 0.0.1");
    scenebuffer = FrameBuffer::Create(1000.0f, 1000.0f);
    uiEngine.Init(window->GetGLFWWindow());
    DefineUI();

    ShaderRegistry::AddShader(
        "main",
        "/Users/anirban/Documents/Code/engine/editor/Shaders/"
        "vertex_shader.glsl",
        "/Users/anirban/Documents/Code/engine/editor/Shaders/"
        "fragment_shader.glsl");

    ShaderRegistry::AddShader(
        "skybox",
        "/Users/anirban/Documents/Code/engine/editor/Shaders/skybox/"
        "skybox_vertex_shader.glsl",
        "/Users/anirban/Documents/Code/engine/editor/Shaders/skybox/"
        "skybox_fragment_shader.glsl");

    ShaderRegistry::AddShader(
        "grid",
        "/Users/anirban/Documents/Code/engine/editor/Shaders/grid/"
        "grid_vertex_shader.glsl",
        "/Users/anirban/Documents/Code/engine/editor/Shaders/grid/"
        "grid_fragment_shader.glsl");

    Entity skybox = scene.CreateEntity("skybox");
    m_MainCameraEntity = scene.CreateEntity("main_camera");

    scene.AddComponent<SkyboxComponent>(
        skybox, TextureCube::CreateFromCrossLayout(
                    "/Users/anirban/Documents/Code/engine/editor/Cubemaps/"
                    "plainscubemap.png"));

    scene.AddComponent<CameraComponent>(
        m_MainCameraEntity, glm::vec3(0.0f, 100.0f, 100.0f), glm::vec3(0.0f),
        glm::vec4(45.0f, 1.0f, 0.1f, 1000.0f));

    m_SkyboxSystem.Init(scene);

    // m_SkyboxCubemap = TextureCube::CreateFromCrossLayout(
    //     "/Users/anirban/Documents/Code/engine/editor/Cubemaps/"
    //     "plainscubemap.png");

    std::vector<float> model_vertices;
    std::vector<uint32> model_indices;

    // auto mainshader = ShaderProgram::Create(
    //     "/Users/anirban/Documents/Code/engine/editor/Shaders/"
    //     "vertex_shader.glsl",
    //     "/Users/anirban/Documents/Code/engine/editor/Shaders/"
    //     "fragment_shader.glsl");

    // Store shader in scene
    // shaders.push_back(ShaderRegistry::GetShader("main"));

    // Set up material
    auto material =
        std::make_shared<Material>(ShaderRegistry::GetShader("main"));

    // Set up camera
    camera.SetCameraProjection(45.0f, 1.0f, 0.1f, 1000.0f);
    camera.SetCameraPosition(0.0f, 100.0f, 100.0f);
    camera.SetCameraLook(0.0f, 0.0f, 0.0f);

    // Set up light
    light.SetColor(glm::vec3(1.0f));
    light.SetPosition(glm::vec3(0.0, 10.0, 0.0));

    auto default_mesh = Importer::Load(
        "/Users/anirban/Documents/Code/engine/editor/models/"
        "testscene.obj",
        material);

    auto other_mesh = Importer::Load(
        "/Users/anirban/Documents/Code/engine/editor/models/"
        "Chair.obj",
        material);

    auto gun = Importer::Load(
        "/Users/anirban/Documents/Code/engine/editor/models/m4colt.obj",
        material);
    // Create mesh
    AddMesh(default_mesh);
}

void Editor::OnUpdate() {}

void Editor::OnRender() {
    // Render ImGUI Panels
    uiEngine.BeginFrame();
    uiEngine.RenderPanels();
    uiEngine.EndFrame();

    // Set global uniforms on all shaders

    auto shader = ShaderRegistry::GetShader("main");
    shader->Bind();
    shader->SetUniformFloat3("diffuseColor",
                             glm::vec3(1.0f, 1.0f, 1.0f));  // example color
    shader->SetUniformMat4("view", camera.GetView());
    shader->SetUniformMat4("projection", camera.GetProjection());
    shader->SetUniformFloat3("viewPos", camera.GetCameraPosition());
    shader->SetUniformFloat3("lightPos", light.GetPosition());
    shader->SetUniformFloat3("lightColor", light.GetColor());

    scenebuffer->Bind();
    context->Clear();
    context->BeginScene();

    // Render Cubemap
    // m_SkyboxRenderer.Render(context, ShaderRegistry::GetShader("skybox"),
    //                         m_SkyboxCubemap, camera);

    m_CameraSystem.UpdateView(scene);
    m_SkyboxSystem.Render(scene);

    // Render Grid
    // m_GridRenderer.Render(context, ShaderRegistry::GetShader("grid"),
    // camera);

    m_GridSystem.Render(scene);

    for (auto& mesh : meshes) {
        context->SubmitMesh(mesh);
    }
    context->EndScene();

    scenebuffer->Unbind();
}

//----------------------------UI Definition----------------------------//

void Editor::DefineUI() {
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

void Editor::OnKeyPressed(int key) {
    // Retrieve current camera position and look target.
    glm::vec3 position = camera.GetCameraPosition();
    glm::vec3 look = camera.GetCameraLook();

    auto& cameraComponent =
        scene.GetComponent<CameraComponent>(m_MainCameraEntity);

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

        cameraComponent.cameraPosition = glm::vec3(
            position.x + delta.x, position.y + delta.y, position.z + delta.z);
        cameraComponent.cameraLook =
            glm::vec3(look.x + delta.x, look.y + delta.y, look.z + delta.z);
    }
    if (key == KeyEvent::KEY_S) {
        glm::vec3 delta = forward * movementSpeed;
        camera.SetCameraPosition(position.x - delta.x, position.y - delta.y,
                                 position.z - delta.z);
        camera.SetCameraLook(look.x - delta.x, look.y - delta.y,
                             look.z - delta.z);

        cameraComponent.cameraPosition = glm::vec3(
            position.x - delta.x, position.y - delta.y, position.z - delta.z);
        cameraComponent.cameraLook =
            glm::vec3(look.x - delta.x, look.y - delta.y, look.z - delta.z);
    }

    if (key == KeyEvent::KEY_A) {
        glm::vec3 delta = right * movementSpeed;
        camera.SetCameraPosition(position.x - delta.x, position.y - delta.y,
                                 position.z - delta.z);
        camera.SetCameraLook(look.x - delta.x, look.y - delta.y,
                             look.z - delta.z);

        cameraComponent.cameraPosition = glm::vec3(
            position.x - delta.x, position.y - delta.y, position.z - delta.z);
        cameraComponent.cameraLook =
            glm::vec3(look.x - delta.x, look.y - delta.y, look.z - delta.z);
    }

    if (key == KeyEvent::KEY_D) {
        glm::vec3 delta = right * movementSpeed;
        camera.SetCameraPosition(position.x + delta.x, position.y + delta.y,
                                 position.z + delta.z);
        camera.SetCameraLook(look.x + delta.x, look.y + delta.y,
                             look.z + delta.z);

        cameraComponent.cameraPosition = glm::vec3(
            position.x + delta.x, position.y + delta.y, position.z + delta.z);
        cameraComponent.cameraLook =
            glm::vec3(look.x + delta.x, look.y + delta.y, look.z + delta.z);
    }
}

void Editor::OnMousePressed(int button) {
    if (!state.isFrameBufferPanelHovered) {
        return;
    }
    if (button == MouseEvent::MOUSE_BUTTON_LEFT) {
        std::cout << "Left mouse button clicked!" << std::endl;
        leftMouseDown = true;
        firstMouse = true;
    }
}

void Editor::OnMouseReleased(int button) {
    if (!state.isFrameBufferPanelHovered) {
        return;
    }
    if (button == MouseEvent::MOUSE_BUTTON_LEFT) {
        std::cout << "Left mouse button released!" << std::endl;
        leftMouseDown = false;
    }
}

void Editor::OnMouseMoved(double xpos, double ypos) {
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
    auto& cameraComponent =
        scene.GetComponent<CameraComponent>(m_MainCameraEntity);
    cameraComponent.cameraLook = glm::vec3(newLook.x, newLook.y, newLook.z);
}
