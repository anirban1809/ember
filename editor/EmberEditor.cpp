#include "EmberEditor.h"
#include <Core/Events.h>
#include <Core/RenderContext.h>
#include <iostream>

EmberEditor::EmberEditor(int width, int height, const char* title)
    : Application(width, height, title), state(fs, assets) {}

void EmberEditor::OnInit() {
    std::cout << "Initializing Ember Editor 0.0.1" << std::endl;
    auto context = RenderContext::Create();
}

void EmberEditor::OnUpdate() {}

void EmberEditor::OnRender() {}

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
