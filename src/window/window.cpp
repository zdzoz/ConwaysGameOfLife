#include "window.h"

Window::~Window() {
    glfwDestroyWindow(glfw_window);
    glfwTerminate();
}

void Window::init() {
    if (!glfwInit())
        throw std::runtime_error("Failed to initialize glfw!");
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, properties.resizable);
    glfw_window = glfwCreateWindow(properties.width, properties.height, properties.label, nullptr, nullptr);
    if (!glfw_window)
        throw std::runtime_error("Failed to initialize window!");
}

void Window::pollEvents() {
    glfwPollEvents();
}

bool Window::shouldClose() {
    return glfwWindowShouldClose(glfw_window);
}
