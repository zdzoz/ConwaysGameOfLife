#include <iostream>

#include "window.h"
#include "game.h"
#include "texture.h"

#include "components.h"

entt::registry registry;
Game* game;
extern int DISPLAY_WIDTH;
extern int DISPLAY_HEIGHT;

void setup();
void isPressed();
void createCell();
void setGameState();
void deadOrAlive();

int main() {
    auto window = new Window{{
        .label = "Conway's Game of Life",
        .resizable = false,
    }};
    game = new Game(std::move(*window));
    game->init()
            .addStartupSystem({
                setup,
            }).addUpdateSystem({
                isPressed, createCell, setGameState, deadOrAlive
            }).run();
    delete window;
    delete game;
    return 0;
}

struct CellState {
    bool alive{false};
    u32 alive_color = 0xf8f8ffff;
    u32 dead_color = 0x292929ff;
    u32 hover_color = 0x797989ff;
};

struct GameState {
    bool paused{true};
};

void setup() {
    registry.ctx().emplace<GameState>();
    float cell_size = 25.0f;
    int cellsx = DISPLAY_WIDTH / cell_size;
    int cellsy = DISPLAY_HEIGHT / cell_size;
    for (u32 y = 0; y < cellsy; y++) {
        for (u32 x = 0; x < cellsx; x++) {
            auto ent = registry.create();
            auto& cs = registry.emplace<CellState>(ent);
            registry.emplace<GridId>(ent, x, y);
            auto& press = registry.emplace<Pressable>(ent);
            press.zone = glm::vec2(cell_size);
            auto& pos = registry.emplace<Position>(ent);
            pos.y = cell_size * y;
            pos.x = cell_size * x;
            auto& texture = registry.emplace<Texture>(ent);
            texture.width = cell_size;
            texture.height = cell_size;
            texture.color = cs.dead_color;
        }
    }
}

void isPressed() {
    auto view = registry.view<Pressable, Position>();
    auto [mx, my] = game->getWindow().getMousePos();
    auto [left, right] = game->getWindow().getMouseClick();

    for (auto [entity, pressable, position] : view.each()) {
        // check if is in zone
        const auto& zone = pressable.zone;
        if ((position.x < mx && mx < position.x + zone.x) && (position.y < my && my < position.y + zone.y)) {
            pressable.left = left;
            pressable.right = right;
            pressable.hover = true;
        } else {
            // not in zone
            pressable.hover = false;
        }
    }
}

void createCell() {
    auto view = registry.view<Pressable, CellState, Texture>();
    for (auto [ent, pressable, cs, text] : view.each()) {
        if (pressable.left || pressable.right) {
            cs.alive = pressable.left;
//            if (pressable.left) cs.alive = !cs.alive;
//            pressable.left = false;
        }

        text.color = cs.alive ? cs.alive_color : cs.dead_color;
        if (!cs.alive && pressable.hover)
            text.color = cs.hover_color;
    }
}

void setGameState() {
    auto& state = registry.ctx().get<GameState>();
    bool space = game->getWindow().getSpace();

    if (space)
        state.paused = !state.paused;
}

void deadOrAlive() {
    auto view = registry.view<GridId, CellState>();
    auto state = registry.ctx().get<GameState>();
    if (state.paused) return;

    for (auto [ent, id, cs] : view.each()) {
        fmt::print("Not paused\n");

    }
}