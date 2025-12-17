#include "Monster_slime_boss.h"

ALGIF_ANIMATION* Monster_slime_boss::slime_gif = nullptr;

Monster_slime_boss::Monster_slime_boss() : Monster()
{
}

void Monster_slime_boss::load_resources()
{
    if (!slime_gif)
        slime_gif = algif_load_animation("assets/gif/monster/lv1_slime_sp.gif");
}

void Monster_slime_boss::unload_resources()
{
    if (slime_gif) {
        algif_destroy_animation(slime_gif);
        slime_gif = nullptr;
    }
}

void Monster_slime_boss::load_gif()
{
    gif = slime_gif;
}

void Monster_slime_boss::init(int startX, int startY)
{
    x = startX;
    y = startY;
    speed = 2.0f;
    
    base_hp = 20000;
    hp_max = hp = base_hp;
    damagePerSecond = 200000000;
    setHP(120000);
    
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