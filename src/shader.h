#pragma once

class Shader {
public:
    Shader(const char* vertex, const char* frag) {
        _program = createProgram(vertex, frag);
    }

    ~Shader();

    inline auto program() -> bgfx::ProgramHandle& { return _program; }

private:
    auto createProgram(const char* vertex, const char* frag) -> bgfx::ProgramHandle;
    bgfx::ProgramHandle _program;
};

//class VertexBuffer {
//public:
//    template<typename V>
//    VertexBuffer(bgfx::VertexLayout layout, V& vertices) {
//        _vertex_buffer = bgfx::createVertexBuffer(bgfx::copy(vertices, sizeof(vertices)), layout);
//        if (!bgfx::isValid(_vertex_buffer))
//            throw std::runtime_error("Failed to create vertex buffer!");
//    }
//
//    ~VertexBuffer();
//
//    inline auto get() -> bgfx::VertexBufferHandle& {
//        return _vertex_buffer;
//    }
//
//    static auto createLayout() -> bgfx::VertexLayout {
//        bgfx::VertexLayout layout;
//        return layout.begin(bgfx::getRendererType());
//    }
//
//private:
//    bgfx::VertexBufferHandle _vertex_buffer;
//};
//
//class IndexBuffer {
//public:
//    template<typename I>
//    IndexBuffer(I& indices) {
//        _index_buffer = bgfx::createIndexBuffer(bgfx::copy(indices, sizeof(indices)));
//        if (!bgfx::isValid(_index_buffer))
//            throw std::runtime_error("Failed to create index buffer!");
//    }
//
//    ~IndexBuffer();
//
//    inline auto get() -> bgfx::IndexBufferHandle& {
//        return _index_buffer;
//    }
//
//private:
//    bgfx::IndexBufferHandle _index_buffer;
//};