#pragma once

#ifdef VULKAN
    #define GLFW_INCLUDE_VULKAN
#endif

#include "GLFW/glfw3.h"
#include "GLFW/glfw3native.h"

#ifdef APPLE
    #include "metal_window.h"
#endif

class Window {
public:
    struct WindowProps {
        const char* label{"Default Window"};
        int width{800}, height{600};
        bool resizable{true};
    };
    Window(WindowProps props) : properties(props) { init(); }
    Window(const Window&) = delete;
    Window(const Window&& win) : glfw_window(win.glfw_window), properties(win.properties) {}
    Window& operator=(const Window& win) = delete;
    ~Window();

    inline void* getNativeWindow() {
        #ifdef __APPLE__
        return bgfx::getMetalLayer(glfwGetCocoaWindow(glfw_window));
        #elif defined(_WIN32)
        return glfwGetWin32Window(glfw_window);
        #else
        throw std::runtime_error("Unable to get native window!");
        return nullptr; // FIXME
        #endif
    };
    inline GLFWwindow* getGlfwWindow() { return glfw_window; }

    void pollEvents();
    bool shouldClose();

    inline int width() { return properties.width; }
    inline int heigth() { return properties.height; }
    inline auto getMousePos() const -> const std::tuple<double, double> {
        double x, y;
        glfwGetCursorPos(glfw_window, &x, &y);
        return {x, y};
    }

    inline auto getMouseClickDown() const -> const std::tuple<bool, bool> {
        static int oldState_left = GLFW_RELEASE;
        int newState_left = glfwGetMouseButton(glfw_window, GLFW_MOUSE_BUTTON_LEFT);
        auto left = newState_left == GLFW_PRESS && oldState_left == GLFW_RELEASE;
        oldState_left = newState_left;

        static int oldState_right = GLFW_RELEASE;
        int newState_right = glfwGetMouseButton(glfw_window, GLFW_MOUSE_BUTTON_RIGHT);
        auto right = newState_right == GLFW_PRESS && oldState_right == GLFW_RELEASE;
        oldState_right = newState_right;

        return {left, right};
    }

    inline auto getMouseClick() const -> const std::tuple<bool, bool> {
        bool left = glfwGetMouseButton(glfw_window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;
        bool right = glfwGetMouseButton(glfw_window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS;

        return {left, right};
    }

    inline auto getSpace() const -> bool {
        static int oldState = GLFW_RELEASE;
        int newState = glfwGetKey(glfw_window, GLFW_KEY_SPACE);
        auto space = newState == GLFW_PRESS && oldState == GLFW_RELEASE;
        oldState = newState;
        return space;
    }

    inline auto getFrameBufferSize() -> decltype(auto) {
        int w, h;
        glfwGetFramebufferSize(glfw_window, &w, &h);
        return std::make_tuple(w, h);
    };
private:
    void init();
    GLFWwindow* glfw_window;
    WindowProps properties;
};
