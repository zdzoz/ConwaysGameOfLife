#pragma once

#include "window.h"

#include <variant>

class Game {
public:
    explicit Game(const Window&& win) : window(std::move(win)) {}
    ~Game();

    Game& init();
    void run();

    Game& addStartupSystem(const std::vector<std::function<void()>>&& fun);

    Game& addUpdateSystem(const std::vector<std::function<void()>>&& fun);
    Game& addShutdownSystem(const std::vector<std::function<void()>>&& fun);
    inline const Window& getWindow() { return window; }
private:
    Window window;
    bgfx::ViewId viewId{0};
    std::vector<std::function<void()>> startup_functions;
    std::vector<std::function<void()>> update_functions;
    std::vector<std::function<void()>> shutdown_functions;

    void init_graphicAPI();

    void runStartupSystems();
    void runUpdateSystems();
    void runShutdownSystems();

// DEBUG //
    struct DebugOptions {
        enum {
            NoRenders = BGFX_DEBUG_IFH,
            Profiler = BGFX_DEBUG_PROFILER,
            Stats = BGFX_DEBUG_STATS,
            Text = BGFX_DEBUG_TEXT,
            Wireframe = BGFX_DEBUG_WIREFRAME
        };
    };

    u32 debugOptions{0};
    void toggleStates();

    inline void addDebugOptions(u32 options) {
        debugOptions |= options;
        bgfx::setDebug(debugOptions);
    };

    inline void removeDebugOptions(u32 options) {
        debugOptions ^= options;
        bgfx::setDebug(debugOptions);
    }
    // DEBUG END //
};
