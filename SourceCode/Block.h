#ifndef BLOCK_H_INCLUDED
#define BLOCK_H_INCLUDED


#include <allegro5/allegro_font.h>

enum class BLOCKTYPE {
    ADD,        // +
    SUB,        // -
    MUL,        // x
    WEAPON_LEVEL_UP
};

class Block
{
public:
    Block() : x(0), y(0), width(50), height(50), speed(5), state(STATE::MOVE) {}

    void init(int startX, int startY, int w, int h, int moveSpeed, BLOCKTYPE t, int val);
    void update();
    void draw();

    static void setupBitmapResource(ALLEGRO_BITMAP* lv2, ALLEGRO_BITMAP* lv3, ALLEGRO_BITMAP* lv4, ALLEGRO_BITMAP* lv5, ALLEGRO_BITMAP* lv6, ALLEGRO_BITMAP* lv7);
    
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

    static ALLEGRO_BITMAP* weapon_lv2_bitmap;
    static ALLEGRO_BITMAP* weapon_lv3_bitmap;
    static ALLEGRO_BITMAP* weapon_lv4_bitmap;
    static ALLEGRO_BITMAP* weapon_lv5_bitmap;
    static ALLEGRO_BITMAP* weapon_lv6_bitmap;
    static ALLEGRO_BITMAP* weapon_lv7_bitmap;

    int x, y;
    int width, height;
    int speed;  // 往左移動速度（正值）
};

#endif
