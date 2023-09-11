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

std::vector<std::vector<entt::entity>> cells;
void setup() {
    registry.ctx().emplace<GameState>();
    float cell_size = 25.0f;
    int cellsx = DISPLAY_WIDTH / cell_size;
    int cellsy = DISPLAY_HEIGHT / cell_size;
    cells.reserve(cellsy);
    for (u32 y = 0; y < cellsy; y++) {
        std::vector<entt::entity> _cells;
        _cells.reserve(cellsx);
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

            _cells.push_back(ent);
        }
        cells.emplace_back(_cells);
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

auto getSurroundingCells(GridId id) -> std::array<std::optional<entt::entity>, 8> {
    std::array<std::optional<entt::entity>, 8> ret{};
    bool left = id.x == 0;
    bool right = id.x == cells[0].size() - 1;
    bool top = id.y == 0;
    bool bot = id.y == cells.size() - 1;

    ret[0] = left || top   ? std::nullopt : std::make_optional(cells[id.y - 1][id.x - 1]); // tl
    ret[1] = top           ? std::nullopt : std::make_optional(cells[id.y - 1][id.x    ]); // tm
    ret[2] = right || top  ? std::nullopt : std::make_optional(cells[id.y - 1][id.x + 1]); // tr
    ret[3] = left          ? std::nullopt : std::make_optional(cells[id.y    ][id.x - 1]); // ml
    ret[4] = right         ? std::nullopt : std::make_optional(cells[id.y    ][id.x + 1]); // mr
    ret[5] = left || bot   ? std::nullopt : std::make_optional(cells[id.y + 1][id.x - 1]); // bl
    ret[6] = bot           ? std::nullopt : std::make_optional(cells[id.y + 1][id.x    ]); // bm
    ret[7] = right || bot  ? std::nullopt : std::make_optional(cells[id.y + 1][id.x + 1]); // br

    return ret;
}

void deadOrAlive() {
    auto view = registry.view<GridId, CellState>();
    auto state = registry.ctx().get<GameState>();
    static double oldTime = 0.0f;
    double time = glfwGetTime();
    if (state.paused || time - oldTime < .5f) return;

    std::vector<entt::entity> killList;
    std::vector<entt::entity> liveList;
    // GAME LOGIC
    for (auto [ent, id, cs] : view.each()) {
        u8 live = 0;
        auto surrounding_entities = getSurroundingCells(id);
        for (auto& s_ent_id : surrounding_entities) {
            if (s_ent_id) {
                auto [s_ent, gid] = registry.get<CellState, GridId>(s_ent_id.value());
                if (s_ent.alive) live++;
            }
        }
        // Any live cell with two or three live neighbours survives.
        // Any dead cell with three live neighbours becomes a live cell.
        // All other live cells die in the next generation. Similarly, all other dead cells stay dead.
        if (cs.alive && (live == 2 || live == 3)) {}
        else if (!cs.alive && live == 3) {
            liveList.push_back(ent);
        } else {
            killList.push_back(ent);
        }
    }

    for (auto& e : killList) {
        auto& cs = registry.get<CellState>(e);
        cs.alive = false;
    }

    for (auto& e : liveList) {
        auto& cs = registry.get<CellState>(e);
        cs.alive = true;
    }

    oldTime = time;
}