
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
#include "ECS/Components/LightComponent.h"
#include "ECS/Components/MeshComponent.h"
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
      m_GridSystem(context),
      m_MeshSystem(context),
      m_SceneManager(scene) {}

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
        skybox,
        "/Users/anirban/Documents/Code/engine/editor/Cubemaps/"
        "cubemap3.png");

    scene.AddComponent<CameraComponent>(
        m_MainCameraEntity, glm::vec3(0.0f, 100.0f, 100.0f), glm::vec3(0.0f),
        glm::vec4(60.0f, 1.0f, 0.1f, 1000.0f));

    m_SkyboxSystem.Init(scene);

    Entity e2 = scene.CreateEntity("default_mesh");
    e2.AddComponent<MeshComponent>(Importer::LoadMeshComponent(
        "/Users/anirban/Documents/Code/engine/editor/models/"
        "Chair.obj"));

    Entity light1 = scene.CreateEntity("default_point_light");
    LightComponent lightComponent = LightComponent();
    lightComponent.lightColor = glm::vec3(1.0f);
    lightComponent.lightPosition = glm::vec3(0.0f, 10.0f, 0.0f);
    scene.AddComponent<LightComponent>(light1, lightComponent);
}

void Editor::OnUpdate() {}

void Editor::OnRender() {
    // Render ImGUI Panels
    uiEngine.BeginFrame();
    uiEngine.RenderPanels();
    uiEngine.EndFrame();

    // bind scenebuffer
    scenebuffer->Bind();

    // clear rendercontext
    context->Clear();

    m_MeshSystem.Init(scene);

    context->BeginScene();
    {
        m_LightSystem.Update(scene);
        m_SkyboxSystem.Render(scene);
        m_GridSystem.Render(scene);
        m_CameraSystem.UpdateView(scene);
        m_MeshSystem.Render(scene);
    }
    context->EndScene();

    // unbind scenebuffer
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

    lc1->AddElement(CreatePanel<ScenePropsPanel>(state, m_SceneManager), 1);
    lc1->AddElement(CreatePanel<AssetLibraryPanel>(state), 1);
    lc2->AddElement(CreatePanel<FramebufferPanel>("Scene", state, scenebuffer),
                    1);
    lc3->AddElement(CreatePanel<NodePropsPanel>(state, m_SceneManager), 1);

    lc->AddElement(lc1, 3);
    lc->AddElement(lc2, 9);
    lc->AddElement(lc3, 2);

    uiEngine.GetUIManager().AddLayoutContainer(lc);
    uiEngine.GetUIManager().AddPanel(CreatePanel<LoadObjectPanel>(state));
}

//-------------------------Input Handling--------------------------------//

void Editor::OnKeyPressed(int key) {
    auto& cameraComponent =
        scene.GetComponent<CameraComponent>(m_MainCameraEntity);

    glm::vec3 look = cameraComponent.cameraLook;
    glm::vec3 position = cameraComponent.cameraPosition;

    // Calculate forward vector (direction the camera is facing)
    glm::vec3 forward = glm::normalize(look - position);
    // Compute right vector from forward and world up vector.
    glm::vec3 right =
        glm::normalize(glm::cross(forward, glm::vec3(0.0f, 1.0f, 0.0f)));

    if (key == KeyEvent::KEY_W) {
        std::cout << "Moving Forward" << std::endl;
        glm::vec3 delta = forward * movementSpeed;
        cameraComponent.cameraPosition += delta;
        cameraComponent.cameraLook += delta;
    }
    if (key == KeyEvent::KEY_S) {
        glm::vec3 delta = forward * movementSpeed;
        cameraComponent.cameraPosition -= delta;
        cameraComponent.cameraLook -= delta;
    }

    if (key == KeyEvent::KEY_A) {
        glm::vec3 delta = right * movementSpeed;
        cameraComponent.cameraPosition -= delta;
        cameraComponent.cameraLook -= delta;
    }

    if (key == KeyEvent::KEY_D) {
        glm::vec3 delta = right * movementSpeed;
        cameraComponent.cameraPosition += delta;
        cameraComponent.cameraLook += delta;
    }

#ifdef debug

    std::cout << "Camera Position: " << cameraComponent.cameraPosition.x << ","
              << cameraComponent.cameraPosition.y << ","
              << cameraComponent.cameraPosition.z << std::endl;

#endif
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
    auto& cameraComponent =
        scene.GetComponent<CameraComponent>(m_MainCameraEntity);

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

    glm::vec3 newLook = cameraComponent.cameraPosition + front;
    cameraComponent.cameraLook = newLook;
}
