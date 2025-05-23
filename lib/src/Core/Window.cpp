#include "../../include/Core/Window.h"
#include "../../../vendor/glfw-3.4.bin.MACOS/include/GLFW/glfw3.h"
#include <iostream>

Window::Window(int width, int height, const char* title) {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_DEPTH_BITS, 24);

    window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return;
    }

    int bufferWidth, bufferHeight;
    glfwGetFramebufferSize(window, &bufferWidth, &bufferHeight);
    glfwMakeContextCurrent(window);
    glfwSetWindowUserPointer(window, this);
    glfwSetKeyCallback(window, KeyCallbackInternal);
    glfwSetMouseButtonCallback(window, MouseButtonCallbackInternal);
    glfwSetCursorPosCallback(window, MouseCursorCallbackInternal);
}

GLFWwindow* Window::GetGLFWWindow() const { return window; }

Window::~Window() {
    glfwDestroyWindow(window);
    glfwTerminate();
}

bool Window::ShouldClose() const { return glfwWindowShouldClose(window); }

void Window::PollEvents() { glfwPollEvents(); }

void Window::SwapBuffers() { glfwSwapBuffers(window); }

void Window::SetKeyCallback(KeyCallback callback) { keyCallback = callback; }
void Window::SetMouseButtonCallback(MouseButtonCallback callback) {
    mouseButtonCallback = callback;
}

void Window::SetMouseCursorCallback(MouseCursorCallback callback) {
    mouseCursorCallback = callback;
}

void Window::KeyCallbackInternal(GLFWwindow* window, int key, int scancode,
                                 int action, int mods) {
    Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));
    if (win) {
        if (win->keyCallback) {
            win->keyCallback(key, action);
        } else {
            std::cout << "Key Callback is NOT set!" << std::endl;
        }
    } else {
        std::cout << "Window pointer is NULL!" << std::endl;
    }
}

void Window::MouseButtonCallbackInternal(GLFWwindow* window, int button,
                                         int action, int mods) {
    Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));

    if (win) {
        if (win->mouseButtonCallback) {
            win->mouseButtonCallback(button, action);
        } else {
            std::cout << "Mouse Callback is NOT set!" << std::endl;
        }
    } else {
        std::cout << "Window pointer is NULL!" << std::endl;
    }
}

void Window::MouseCursorCallbackInternal(GLFWwindow* window, double xpos,
                                         double ypos) {
    Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));

    if (win) {
        if (win->mouseCursorCallback) {
            win->mouseCursorCallback(xpos, ypos);
        } else {
            std::cout << "Mouse Callback is NOT set!" << std::endl;
        }
    } else {
        std::cout << "Window pointer is NULL!" << std::endl;
    }
}