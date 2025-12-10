#include "Monster_Lv2_zombie.h"

ALGIF_ANIMATION* Monster_Lv2_zombie::slime_gif = nullptr;

Monster_Lv2_zombie::Monster_Lv2_zombie() : Monster()
{
}

void Monster_Lv2_zombie::load_resources()
{
    if (!slime_gif)
        slime_gif = algif_load_animation("assets/gif/monster/lv2_zombie.gif");
}

void Monster_Lv2_zombie::unload_resources()
{
    if (slime_gif) {
        algif_destroy_animation(slime_gif);
        slime_gif = nullptr;
    }
}

void Monster_Lv2_zombie::load_gif()
{
    gif = slime_gif;
}

void Monster_Lv2_zombie::init(int startX, int startY)
{
    x = startX;
    y = startY;
    speed = 2.0f;
    
    base_hp = 40;
    hp_max = hp = base_hp;
    
    load_gif();
    
    if (gif)
    {
        ALLEGRO_BITMAP* firstFrame = algif_get_bitmap(gif, al_get_time());
        if (firstFrame)
        {
            width  = al_get_bitmap_width(firstFrame) * 1.2;
            height = al_get_bitmap_height(firstFrame) * 1.2;
        }
    }
    
    // ⭐ 初始化縮放
    updateScale();
}