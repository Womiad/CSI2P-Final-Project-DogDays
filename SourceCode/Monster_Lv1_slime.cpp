#include "Monster_Lv1_slime.h"

ALGIF_ANIMATION* Monster_Lv1_slime::slime_gif = nullptr;

Monster_Lv1_slime::Monster_Lv1_slime() : Monster()
{
}

void Monster_Lv1_slime::load_resources()
{
    if (!slime_gif)
        slime_gif = algif_load_animation("assets/gif/monster/lv1_slime.gif");
}

void Monster_Lv1_slime::unload_resources()
{
    if (slime_gif) {
        algif_destroy_animation(slime_gif);
        slime_gif = nullptr;
    }
}

void Monster_Lv1_slime::load_gif()
{
    gif = slime_gif;
}

void Monster_Lv1_slime::init(int startX, int startY)
{
    x = startX;
    y = startY;
    speed = 2.0f;
    
    base_hp = 20;
    hp_max = hp = base_hp;
    
    load_gif();
    
    if (gif)
    {
        ALLEGRO_BITMAP* firstFrame = algif_get_bitmap(gif, al_get_time());
        if (firstFrame)
        {
            width  = al_get_bitmap_width(firstFrame);
            height = al_get_bitmap_height(firstFrame);
        }
    }
    
    // ⭐ 初始化縮放
    updateScale();
}