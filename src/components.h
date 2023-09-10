
struct Name {
    std::string name;
};

struct GridId {
    int x, y;
};

struct Position {
    float x, y;
};

struct Color {
    float r{}, g{}, b{}, a{};

    Color() = default;

    Color(u8 r, u8 g, u8 b, u8 a) : r(r / (float) 0xff), g(g / (float) 0xff), b(b / (float) 0xff),
                                    a(a / (float) 0xff) {}

    Color(u32 col) :
            r((float) ((col >> 0x18) & 0xff) / (float) 0xff),
            g((float) ((col >> 0x10) & 0xff) / (float) 0xff),
            b((float) ((col >> 0x08) & 0xff) / (float) 0xff),
            a((float) (col & 0xff) / (float) 0xff) {}

    void* operator&() {
        return this;
    }
};

struct Pressable {
    glm::vec2 zone{50.0f, 50.0f};
    bool left{false};
    bool right{false};
    bool hover{false};
};
