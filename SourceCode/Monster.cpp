// Monster.cpp
#include "Monster.h"
#include <allegro5/allegro_primitives.h>

// 初始化靜態成員
ALGIF_ANIMATION* Monster::gif_lv1 = nullptr;
ALGIF_ANIMATION* Monster::gif_lv2 = nullptr;
ALGIF_ANIMATION* Monster::gif_lv3 = nullptr;
ALGIF_ANIMATION* Monster::gif_lv4 = nullptr;
ALGIF_ANIMATION* Monster::gif_lv5 = nullptr;

Monster::Monster()
{
    x = y = 0;
    width = height = 50;
    speed = 2;
    hp = hp_max = 10;
    gif = nullptr;
}

void Monster::load_resources()
{
    // 在遊戲開始時載入所有怪物的 GIF
    if (!gif_lv1) gif_lv1 = algif_load_animation("assets/gif/monster/lv1_slime.gif");
    // if (!gif_lv2) gif_lv2 = algif_load_animation("assets/gif/monster/lv2.gif");
    // if (!gif_lv3) gif_lv3 = algif_load_animation("assets/gif/monster/lv3.gif");
    // if (!gif_lv4) gif_lv4 = algif_load_animation("assets/gif/monster/lv4.gif");
    // if (!gif_lv5) gif_lv5 = algif_load_animation("assets/gif/monster/lv5.gif");
}

void Monster::unload_resources()
{
    // 遊戲結束時釋放資源
    if (gif_lv1) { algif_destroy_animation(gif_lv1); gif_lv1 = nullptr; }
    // if (gif_lv2) { algif_destroy_animation(gif_lv2); gif_lv2 = nullptr; }
    // if (gif_lv3) { algif_destroy_animation(gif_lv3); gif_lv3 = nullptr; }
    // if (gif_lv4) { algif_destroy_animation(gif_lv4); gif_lv4 = nullptr; }
    // if (gif_lv5) { algif_destroy_animation(gif_lv5); gif_lv5 = nullptr; }
}

void Monster::init(int startX, int startY, MonsterTYPE t)
{
    type = t;
    x = startX;
    y = startY;

    // 直接使用預載入的 GIF（不需要再次載入）
    switch (type)
    {
        case MonsterTYPE::LV1_SLIME:
            gif = gif_lv1;
            speed = 2.0f;
            hp_max = hp = 20;
            break;

        // case MonsterTYPE::LV2:
        //     gif = gif_lv2;
        //     speed = 2.6f;
        //     hp_max = hp = 40;
        //     break;

        // case MonsterTYPE::LV3:
        //     gif = gif_lv3;
        //     speed = 3.0f;
        //     hp_max = hp = 80;
        //     break;

        // case MonsterTYPE::LV4:
        //     gif = gif_lv4;
        //     speed = 3.5f;
        //     hp_max = hp = 150;
        //     break;

        // case MonsterTYPE::LV5:
        //     gif = gif_lv5;
        //     speed = 4.0f;
        //     hp_max = hp = 250;
        //     break;
    }

    if (gif)
    {
        ALLEGRO_BITMAP* firstFrame = algif_get_bitmap(gif, al_get_time());
        if (firstFrame)
        {
            width  = al_get_bitmap_width(firstFrame);
            height = al_get_bitmap_height(firstFrame);
        }
    }
}

void Monster::update()
{
    // 往左移動
    x -= speed;
}

void Monster::draw()
{
    if (!gif) return;

    // 取得目前動畫幀
    ALLEGRO_BITMAP* bmp = algif_get_bitmap(gif, al_get_time());
    if (!bmp) return;

    // 因為 gif 往上畫 (腳在 y)
    al_draw_bitmap(bmp, x, y - height, 0);

    // -------------------------
    //    血條
    // -------------------------
    float barW = width;
    float barH = 8;

    float barX = x;
    float barY = y - height - 15;

    // 外框
    al_draw_rectangle(barX, barY, barX + barW, barY + barH, al_map_rgb(0,0,0), 2);

    // 血量
    float hpPercent = (float)hp / hp_max;
    al_draw_filled_rectangle(barX, barY, barX + barW * hpPercent, barY + barH, al_map_rgb(255,0,0));
}