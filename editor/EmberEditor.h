#ifndef __EMBEREDITOR_H__
#define __EMBEREDITOR_H__

#include <Core/Application.h>
#include "./include/UI/ApplicationState.h"

class EmberEditor : public Application {
   public:
    EmberEditor(int, int, const char*);

   protected:
    void OnInit() override;
    void OnKeyPressed(int key) override;
    void OnMousePressed(int button) override;
    void OnMouseReleased(int button) override;
    void OnMouseMoved(double xpos, double ypos) override;
    void OnUpdate() override;
    void OnRender() override;

   private:
    bool value = false;
    bool leftMouseDown = false;
    double lastX = 0.0f;
    double lastY = 0.0f;
    bool firstMouse = false;
    float yaw = -90.0f;  // Initialized so that the initial front is along -Z.
    float pitch = 0.0f;
    float mouseSensitivity = 0.1f;
    float movementSpeed = 1.0f;
    ApplicationState state;
    FileSystem fs;
    std::vector<std::string> assets;
};

#endif  // __EMBEREDITOR_H__