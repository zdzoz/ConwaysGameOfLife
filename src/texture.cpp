//
// Created by Oskar Zdziarski on 9/8/23.
//

#include "texture.h"

#include "game.h"
#include "components.h"

extern Game* game;
extern entt::registry registry;

static Shader* shader;
static bgfx::UniformHandle uModelView;

struct TextureLayout {
    float posx, posy;
    u32 col;
};

extern int DISPLAY_WIDTH;
extern int DISPLAY_HEIGHT;
void drawTextures() {
    auto view = registry.view<Texture, Position>();
    bgfx::VertexLayout layout;
    layout.begin()
        .add(bgfx::Attrib::Position, 2, bgfx::AttribType::Float)
        .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
        .end();
    std::vector<TextureLayout> vertices;
    std::vector<u16> indices;
    u16 i = 0;
    for (auto [ent, texture, position] : view.each()) {
        vertices.push_back({position.x, position.y,                                  texture.color}); // bl
        vertices.push_back({position.x, position.y + texture.height,                 texture.color}); // tl
        vertices.push_back({position.x + texture.width, position.y + texture.height, texture.color}); // tr
        vertices.push_back({position.x + texture.width, position.y,                  texture.color}); // br
        indices.push_back(0 + i);
        indices.push_back(1 + i);
        indices.push_back(2 + i);
        indices.push_back(2 + i);
        indices.push_back(3 + i);
        indices.push_back(0 + i);
        i += 4;
    }
    auto vmem = bgfx::copy(vertices.data(), sizeof(TextureLayout) * vertices.size());
    bgfx::VertexBufferHandle vbuf = bgfx::createVertexBuffer(vmem, layout);
    auto imem = bgfx::copy(indices.data(), sizeof(u16) * indices.size());
    bgfx::IndexBufferHandle ibuf = bgfx::createIndexBuffer(imem);

    auto enc = bgfx::begin();
    enc->setState(BGFX_STATE_WRITE_R | BGFX_STATE_WRITE_G | BGFX_STATE_WRITE_B | BGFX_STATE_WRITE_A);

    auto orthview = glm::ortho(0.0f, (float)DISPLAY_WIDTH, (float)DISPLAY_HEIGHT, 0.0f);
    enc->setUniform(uModelView, glm::value_ptr(orthview));

    enc->setVertexBuffer(0, vbuf);
    enc->setIndexBuffer(ibuf);

    enc->submit(0, shader->program());
    end(enc);

    destroy(vbuf);
    destroy(ibuf);
}

void init_texture() {
    game->addStartupSystem({[]() {
        shader = new Shader("vs_texture", "fs_texture");
        uModelView = bgfx::createUniform("uModelView", bgfx::UniformType::Mat4);
    }}).addUpdateSystem({
            drawTextures
    }).addShutdownSystem({[]() {
        bgfx::destroy(uModelView);
        delete shader;
    }});
}
