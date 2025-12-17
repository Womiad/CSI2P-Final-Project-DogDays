#include "Monster_Lv3_dragon.h"

ALGIF_ANIMATION* Monster_Lv3_dragon::slime_gif = nullptr;

Monster_Lv3_dragon::Monster_Lv3_dragon() : Monster()
{
}

void Monster_Lv3_dragon::load_resources()
{
    if (!slime_gif)
        slime_gif = algif_load_animation("assets/gif/monster/lv3_dragon.gif");
}

void Monster_Lv3_dragon::unload_resources()
{
    if (slime_gif) {
        algif_destroy_animation(slime_gif);
        slime_gif = nullptr;
    }
}

void Monster_Lv3_dragon::load_gif()
{
    gif = slime_gif;
}

void Monster_Lv3_dragon::init(int startX, int startY)
{
    x = startX;
    y = startY;
    speed = 2.0f;
    
    base_hp = 1000;
    hp_max = hp = base_hp;
    damagePerSecond = 1000;
    
    load_gif();
    
    if (gif)
    {
        ALLEGRO_BITMAP* firstFrame = algif_get_bitmap(gif, al_get_time());
        if (firstFrame)
        {
            width  = al_get_bitmap_width(firstFrame) * 1.5;
            height = al_get_bitmap_height(firstFrame) * 1.5;
        }
    }
    
    // ⭐ 初始化縮放
    updateScale();
}