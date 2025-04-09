
#include "Core/Application.h"
#include <GLFW/glfw3.h>
#include <iostream>

Application::Application(int width, int height, const char* title) {
    window = new Window(width, height, title);
    renderer = new Renderer();
    // Set up key event callback
    window->SetKeyCallback([this](int key, int action) {
        if (action == GLFW_REPEAT || action == GLFW_PRESS) {
            OnKeyPressed(key);  // Call the abstracted method
        }
    });

    window->SetMouseButtonCallback([this](int button, int action) {
        if (action == GLFW_PRESS) {
            OnMousePressed(button);  // Call the abstracted method
        }

        if (action == GLFW_RELEASE) {
            OnMouseReleased(button);  // Call the abstracted method
        }
    });

    window->SetMouseCursorCallback(
        [this](double xpos, double ypos) { OnMouseMoved(xpos, ypos); });
}

Application::~Application() {
    delete window;
    delete renderer;
}

void Application::Run() {
    OnInit();

    while (!window->ShouldClose()) {
        window->PollEvents();
        OnUpdate();

        renderer->Clear();
        OnRender();

        window->SwapBuffers();
    }
}
