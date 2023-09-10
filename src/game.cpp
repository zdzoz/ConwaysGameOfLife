//
// Created by Oskar Zdziarski on 9/6/23.
//

#include "game.h"

#include "texture.h"
#include "window.h"

int DISPLAY_WIDTH;
int DISPLAY_HEIGHT;

constexpr u32 CLEAR_COLOR = 0x3333ffff;

extern entt::registry registry;

Game::~Game() {
    bgfx::shutdown();
}

Game& Game::init() {
    DISPLAY_WIDTH = window.width();
    DISPLAY_HEIGHT = window.heigth();
    init_graphicAPI();
    init_texture();
    return *this;
}

void Game::run() {
    runStartupSystems();

    while (!window.shouldClose()) {
        window.pollEvents();

//        auto [display_w, display_h] = window.getFrameBufferSize();
        DISPLAY_WIDTH = window.width();
        DISPLAY_HEIGHT = window.heigth();
        bgfx::reset(DISPLAY_WIDTH, DISPLAY_HEIGHT, BGFX_RESET_VSYNC);
        bgfx::setViewRect(viewId, 0, 0, bgfx::BackbufferRatio::Equal);

        #ifndef NDEBUG
        toggleStates();
        bgfx::dbgTextClear();
        const bgfx::Stats* stats = bgfx::getStats();
        bgfx::dbgTextPrintf(0, 0, 0x0f, fmt::format("Backbuffer Size: ({}px, {}px)", stats->width, stats->height).c_str());
        #endif

        // Rendering
//        auto enc = bgfx::begin();
//        enc->touch(viewId); // ensure frame is rendered
//        enc->setState(BGFX_STATE_WRITE_R | BGFX_STATE_WRITE_G | BGFX_STATE_WRITE_B | BGFX_STATE_WRITE_A);

        auto update_sys = std::thread([&]() { runUpdateSystems(); });

//        bgfx::end(enc);

        bgfx::frame(); // swap buffers
        update_sys.join();
    }
    runShutdownSystems();
}

void Game::init_graphicAPI() {
    bgfx::PlatformData pd{};
    pd.nwh = window.getNativeWindow();

    bgfx::Init init;
    init.type = bgfx::RendererType::Count;
    init.vendorId = BGFX_PCI_ID_NONE; // auto
    init.deviceId = 0; // Maybe should choose devices?
#ifndef NDEBUG
    init.debug = true;
    init.profile = true;
#endif
    init.platformData = pd;
    init.resolution.format = bgfx::TextureFormat::Count;
    init.resolution.width = window.width();
    init.resolution.height = window.heigth();
    init.resolution.reset = BGFX_RESET_VSYNC; // TODO: Allow disabling vsync
    init.resolution.numBackBuffers = 2;
    init.resolution.maxFrameLatency = 0;
    if (!bgfx::init(init))
        throw std::runtime_error("Failed to initialize graphics api!");

    const char* str = "[Rendering API]";
    switch (bgfx::getRendererType()) {
        case bgfx::RendererType::Direct3D11: LOG_ERROR("{} Direct3D11\n", str); break;
        case bgfx::RendererType::Direct3D12: LOG_ERROR("{} Direct3D12\n", str); break;
        case bgfx::RendererType::Metal: LOG_ERROR("{} Metal\n", str); break;
        case bgfx::RendererType::OpenGLES: LOG_ERROR("{} OpenGLES\n", str); break;
        case bgfx::RendererType::OpenGL: LOG_ERROR("{} OpenGL\n", str); break;
        case bgfx::RendererType::Vulkan: LOG_ERROR("{} Vulkan\n", str); break;
        case bgfx::RendererType::WebGPU: LOG_ERROR("{} WebGPU\n", str); break;
        default:
            throw std::runtime_error(fmt::format("{} ERROR: Failed to select rendering API!", str));
    }

    addDebugOptions(DebugOptions::Text);

    bgfx::setViewClear(viewId, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, CLEAR_COLOR);
}

Game& Game::addStartupSystem(const std::vector<std::function<void()>>&& fun) {
    for (const auto& f : fun) {
        startup_functions.emplace_back(f);
    }
    return *this;
}

Game& Game::addUpdateSystem(const std::vector<std::function<void()>>&& fun) {
    for (const auto& f : fun) {
        update_functions.emplace_back(f);
    }
    return *this;
}

Game& Game::addShutdownSystem(const std::vector<std::function<void()>>&& fun) {
    for (const auto& f : fun) {
        shutdown_functions.emplace_back(f);
    }
    return *this;
}

void Game::runStartupSystems() {
    for (const auto& fun : startup_functions) {
        fun();
    }
}

void Game::runUpdateSystems() {
    for (const auto& fun : update_functions) {
        fun();
    }
}

void Game::runShutdownSystems() {
    for (const auto& fun : shutdown_functions) {
        fun();
    }
}

void Game::toggleStates() {
    static bool is_down = false;
    if (glfwGetKey(window.getGlfwWindow(), GLFW_KEY_SLASH) == GLFW_PRESS && !is_down) {
        debugOptions = debugOptions & DebugOptions::Stats ? DebugOptions::Text : DebugOptions::Text | DebugOptions::Stats;
        addDebugOptions(debugOptions);
        is_down = true;
    }
    if (glfwGetKey(window.getGlfwWindow(), GLFW_KEY_SLASH) == GLFW_RELEASE) is_down = false;
}
