#ifndef __EMBEREDITOR_H__
#define __EMBEREDITOR_H__

#include "Core/Application.h"
#include "Core/Mesh.h"
#include "Core/RenderContext.h"
#include "Core/ShaderProgram.h"
#include "Core/ShaderRegistry.h"
#include "ECS/Entity.h"
#include "ECS/Entity.inl"
#include "ECS/Systems/CameraSystem.h"
#include "ECS/Systems/GridSystem.h"
#include "ECS/Systems/LightSystem.h"
#include "ECS/Systems/MeshSystem.h"
#include "ECS/Systems/SkyboxSystem.h"
#include "UI/ApplicationState.h"
#include "UI/GridRenderer.h"
#include "UI/SkyboxRenderer.h"
#include "UI/TextureCube.h"
#include "UI/UIEngine.h"
#include <memory>
template <typename T>
using Rc = std::shared_ptr<T>;

class Editor : public Application {
   public:
    Editor(int, int, const char*);

   protected:
    void OnInit() override;
    void OnKeyPressed(int key) override;
    void OnMousePressed(int button) override;
    void OnMouseReleased(int button) override;
    void OnMouseMoved(double xpos, double ypos) override;
    void OnUpdate() override;
    void OnRender() override;
    void AddMesh(Mesh&);

   private:
    void DefineUI();

    // UI controls
    bool value = false;
    bool leftMouseDown = false;
    double lastX = 0.0f;
    double lastY = 0.0f;
    bool firstMouse = false;
    float yaw = -90.0f;  // Initialized so that the initial front is along -Z.
    float pitch = 0.0f;
    float mouseSensitivity = 0.1f;
    float movementSpeed = 1.0f;

    // State Management
    ApplicationState state;
    FileSystem fs;
    std::vector<std::string> assets;
    std::shared_ptr<RenderContext> context;
    std::shared_ptr<FrameBuffer> scenebuffer;
    UIEngine uiEngine;
    std::vector<Mesh> meshes;

    std::vector<Rc<ShaderProgram>> shaders;
    GridRenderer m_GridRenderer;
    SkyboxRenderer m_SkyboxRenderer;
    Rc<TextureCube> m_SkyboxCubemap;

    // systems
    SkyboxSystem m_SkyboxSystem;
    CameraSystem m_CameraSystem;
    GridSystem m_GridSystem;
    MeshSystem m_MeshSystem;
    LightSystem m_LightSystem;

    Entity m_MainCameraEntity;
    Scene scene;
};

#endif  // __EMBEREDITOR_H__
