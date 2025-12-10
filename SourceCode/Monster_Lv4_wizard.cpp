#include "Monster_Lv4_wizard.h"

ALGIF_ANIMATION* Monster_Lv4_wizard::slime_gif = nullptr;

Monster_Lv4_wizard::Monster_Lv4_wizard() : Monster()
{
}

void Monster_Lv4_wizard::load_resources()
{
    if (!slime_gif)
        slime_gif = algif_load_animation("assets/gif/monster/lv4_wizard.gif");
}

void Monster_Lv4_wizard::unload_resources()
{
    if (slime_gif) {
        algif_destroy_animation(slime_gif);
        slime_gif = nullptr;
    }
}

void Monster_Lv4_wizard::load_gif()
{
    gif = slime_gif;
}

void Monster_Lv4_wizard::init(int startX, int startY)
{
    x = startX;
    y = startY;
    speed = 2.0f;
    
    base_hp = 60;
    hp_max = hp = base_hp;
    
    load_gif();
    
    if (gif)
    {
        ALLEGRO_BITMAP* firstFrame = algif_get_bitmap(gif, al_get_time());
        if (firstFrame)
        {
            width  = al_get_bitmap_width(firstFrame) * 1.75;
            height = al_get_bitmap_height(firstFrame) * 1.75;
        }
    }
    
    // ⭐ 初始化縮放
    updateScale();
}