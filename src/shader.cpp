#include "shader.h"

#include <glm/glm.hpp>

#include <shaders/all.h>

Shader::~Shader() {
    bgfx::destroy(_program);
}

auto Shader::createProgram(const char* vertex, const char* frag) -> bgfx::ProgramHandle {
    auto render_type = bgfx::getRendererType();

    auto vs = bgfx::createEmbeddedShader(embedded_shaders, render_type, vertex);
    if (!bgfx::isValid(vs))
        throw std::runtime_error(fmt::format("Failed to create vertex shader: {}", vertex));

    auto fs = bgfx::createEmbeddedShader(embedded_shaders, render_type, frag);
    if (!bgfx::isValid(fs))
        throw std::runtime_error(fmt::format("Failed to create fragment shader: {}", frag));

    _program = bgfx::createProgram(vs, fs, true);
    if (!bgfx::isValid(_program))
        throw std::runtime_error("Failed to create shader program");

    return _program;
}
