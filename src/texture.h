#pragma once

#include "shader.h"

struct Texture {
    float width{50.0f}, height{50.0f};
    u32 color{0xff00ffff};
};

void init_texture();
