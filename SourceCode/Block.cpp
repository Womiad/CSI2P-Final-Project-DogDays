#include "Block.h"
#include <allegro5/allegro_primitives.h>
#include "data/DataCenter.h"
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>   // 如果使用 TTF 字型
#include "data/DataCenter.h"

#include <allegro5/allegro_image.h>

ALLEGRO_BITMAP* Block::weapon_lv2_bitmap = nullptr;
ALLEGRO_BITMAP* Block::weapon_lv3_bitmap = nullptr;
ALLEGRO_BITMAP* Block::weapon_lv4_bitmap = nullptr;
ALLEGRO_BITMAP* Block::weapon_lv5_bitmap = nullptr;
ALLEGRO_BITMAP* Block::weapon_lv6_bitmap = nullptr;
ALLEGRO_BITMAP* Block::weapon_lv7_bitmap = nullptr;


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

void Block::setupBitmapResource(
    ALLEGRO_BITMAP* lv2, 
    ALLEGRO_BITMAP* lv3, 
    ALLEGRO_BITMAP* lv4, 
    ALLEGRO_BITMAP* lv5, 
    ALLEGRO_BITMAP* lv6, 
    ALLEGRO_BITMAP* lv7)
{
      weapon_lv2_bitmap = lv2;  
      weapon_lv3_bitmap = lv3;  
      weapon_lv4_bitmap = lv4;  
      weapon_lv5_bitmap = lv5;  
      weapon_lv6_bitmap = lv6;  
      weapon_lv7_bitmap = lv7;  
}


void Block::update() {
	// DataCenter *DC = DataCenter::get_instance();
    if (state == STATE::MOVE) {
        x -= speed;  // 向左移動
    }
}

void Block::draw() {

    DataCenter *DC = DataCenter::get_instance();

    // 1. 外框顏色
    ALLEGRO_COLOR borderColor;

    switch (type) {
        case BLOCKTYPE::ADD: borderColor = al_map_rgb(0, 0, 120); break;
        case BLOCKTYPE::SUB: borderColor = al_map_rgb(120, 0, 0); break;
        case BLOCKTYPE::MUL: borderColor = al_map_rgb(0, 0, 120); break;

        case BLOCKTYPE::WEAPON_LEVEL_UP:
            borderColor = al_map_rgb(230, 200, 80);  // 金色
            break;
    }

    al_draw_rectangle(x, y, x + width, y + height, borderColor, 4);


    // 2. 漸層背景
    for (int j = 0; j < height; j++) {
        float t = float(j) / height;
        ALLEGRO_COLOR c;

        if (type == BLOCKTYPE::ADD)
            c = al_map_rgb(50 + 80*t, 50*(1-t), 200+55*t);

        else if (type == BLOCKTYPE::SUB)
            c = al_map_rgb(200, 30+150*t, 30);

        else if (type == BLOCKTYPE::MUL)
            c = al_map_rgb(0, 50+180*t, 200*(1-t));

        else if (type == BLOCKTYPE::WEAPON_LEVEL_UP) {
            // 淺金色 → 更淺金色
            // 可以微調這兩組顏色
            unsigned char r = 220 + 20*t;  // 220 → 240
            unsigned char g = 200 + 30*t;  // 200 → 230
            unsigned char b = 140 + 30*t;  // 140 → 170
            c = al_map_rgb(r, g, b);
        }

        al_draw_filled_rectangle(x, y + j, x + width, y + j + 1, c);
    }


    // 3. 畫文字 or 武器圖片
    char buf[16];

    switch(type) {

        case BLOCKTYPE::ADD:
            sprintf(buf, "+%d", value);
            al_draw_text(font, al_map_rgb(255, 255, 255),
                         x + width / 2, y + height / 2 - 16,
                         ALLEGRO_ALIGN_CENTER, buf);
            break;

        case BLOCKTYPE::SUB:
            sprintf(buf, "-%d", value);
            al_draw_text(font, al_map_rgb(255, 255, 255),
                         x + width / 2, y + height / 2 - 16,
                         ALLEGRO_ALIGN_CENTER, buf);
            break;

        case BLOCKTYPE::MUL:
            sprintf(buf, "x%d", value);
            al_draw_text(font, al_map_rgb(255, 255, 255),
                         x + width / 2, y + height / 2 - 16,
                         ALLEGRO_ALIGN_CENTER, buf);
            break;


        case BLOCKTYPE::WEAPON_LEVEL_UP:
        {
            // value 代表武器等級，例如 2～7
            ALLEGRO_BITMAP* bmp = nullptr;

            int level = DC -> idelWeaponLevel;

            if (level == 2) bmp = weapon_lv2_bitmap;
            else if (level == 3) bmp = weapon_lv3_bitmap;
            else if (level == 4) bmp = weapon_lv4_bitmap;
            else if (level == 5) bmp = weapon_lv5_bitmap;
            else if (level == 6) bmp = weapon_lv6_bitmap;
            else if (level == 7) bmp = weapon_lv7_bitmap;
            // else bmp = weapon_lv2_bitmap;

            if (bmp) {
                int bmpW = al_get_bitmap_width(bmp);
                int bmpH = al_get_bitmap_height(bmp);

                // 縮放到 block 中央（不失真）
                float scale = 0.8f * (float)width / bmpW;
                if (scale * bmpH > height * 0.8f)
                    scale = 0.8f * (float)height / bmpH;

                float drawW = bmpW * scale;
                float drawH = bmpH * scale;

                float drawX = x + (width  - drawW) / 2;
                float drawY = y + (height - drawH) / 2;

                al_draw_scaled_bitmap(
                    bmp,
                    0, 0, bmpW, bmpH,
                    drawX, drawY, drawW, drawH,
                    0
                );
            }
            break;
        }
    }
}



