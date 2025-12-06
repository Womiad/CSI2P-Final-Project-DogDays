#include "Block.h"
#include <allegro5/allegro_primitives.h>
#include "data/DataCenter.h"
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>   // 如果使用 TTF 字型

void Block::init(int startX, int startY, int w, int h, int moveSpeed, BLOCKTYPE t, int val) {
    x = startX;
    y = startY;
    width = w;
    height = h;
    speed = moveSpeed;

    state = STATE::MOVE;
    type = t;
    value = val;

    if (!font) { 
        font = al_load_ttf_font("./assets/font/Caviar_Dreams_Bold.ttf", 32, 0);
        if (!font) font = al_create_builtin_font(); // fallback
    }
}


void Block::update() {
	DataCenter *DC = DataCenter::get_instance();
    if (state == STATE::MOVE) {
        x -= speed;  // 向左移動
    }
}

void Block::draw() {
    // ALLEGRO_FONT* font = BlockManager::blockFont;

    // 1. 外框顏色
    ALLEGRO_COLOR borderColor;
    switch (type) {
        case BLOCKTYPE::ADD: borderColor = al_map_rgb(0, 0, 120); break;
        case BLOCKTYPE::SUB: borderColor = al_map_rgb(120, 0, 0); break;
        case BLOCKTYPE::MUL: borderColor = al_map_rgb(0, 0, 120); break;
    }
    al_draw_rectangle(x, y, x + width, y + height, borderColor, 4);

    // 2. 上下漸層
    for (int j = 0; j < height; j++) {
        float t = float(j) / height;
        ALLEGRO_COLOR c;

        if (type == BLOCKTYPE::ADD) c = al_map_rgb(50 + 80*t, 50*(1-t), 200+55*t);
        else if (type == BLOCKTYPE::SUB) c = al_map_rgb(200, 30+150*t, 30);
        else if (type == BLOCKTYPE::MUL) c = al_map_rgb(0, 50+180*t, 200*(1-t));

        al_draw_filled_rectangle(x, y+j, x+width, y+j+1, c);
    }

    // 3. 畫數字（帶符號）
    char buf[16];
    switch(type) {
        case BLOCKTYPE::ADD:
            sprintf(buf, "+%d", value);
            break;
        case BLOCKTYPE::SUB:
            sprintf(buf, "-%d", value);
            break;
        case BLOCKTYPE::MUL:
            sprintf(buf, "x%d", value);
            break;
    }

    al_draw_text(
        font,
        al_map_rgb(255, 255, 255),
        x + width / 2,
        y + height / 2 - 16,
        ALLEGRO_ALIGN_CENTER,
        buf
    );
}


