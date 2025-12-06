#ifndef BLOCK_H_INCLUDED
#define BLOCK_H_INCLUDED


#include <allegro5/allegro_font.h>

enum class BLOCKTYPE {
    ADD,        // +
    SUB,        // -
    MUL         // x
};

class Block
{
public:
    Block() : x(0), y(0), width(50), height(50), speed(5), state(STATE::MOVE) {}

    void init(int startX, int startY, int w, int h, int moveSpeed, BLOCKTYPE t, int val);
    void update();
    void draw();
    
    // --- 新增 getter ---
    int getX() const { return x; }
    int getY() const { return y; }
    int getWidth() const { return width; }
    int getHeight() const { return height; }
    BLOCKTYPE getType() const { return type; }
    int getValue() const { return value; }

private:
    enum class STATE {
        MOVE,
        PAUSE
    };
    STATE state;

    BLOCKTYPE type;   // <<< 新增
    int value;   // <<< 新增

    ALLEGRO_FONT* font = nullptr;  // 新增字體指標

    int x, y;
    int width, height;
    int speed;  // 往左移動速度（正值）
};

#endif
