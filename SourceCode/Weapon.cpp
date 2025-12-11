#include "Weapon.h"
#include "Arrow.h"
#include <allegro5/allegro.h>
#include <algorithm>

void Weapon::init(ALLEGRO_BITMAP* bowImg, ALLEGRO_BITMAP* arrowImg)
{
    bow_img = bowImg;
    arrow_img = arrowImg;
    fire_timer = 0;
    dog_type = 1;  // 預設類型
}

void Weapon::setWeaponImage(ALLEGRO_BITMAP* bow, ALLEGRO_BITMAP* arrow)
{
    this->bow_img = bow;
    this->arrow_img = arrow;
}


// void Weapon::setupBitmap(
//         ALLEGRO_BITMAP* lv1_w, ALLEGRO_BITMAP* lv1_a, 
//         ALLEGRO_BITMAP* lv2_w, ALLEGRO_BITMAP* lv2_a, 
//         ALLEGRO_BITMAP* lv3_w, ALLEGRO_BITMAP* lv3_a, 
//         ALLEGRO_BITMAP* lv4_w, ALLEGRO_BITMAP* lv4_a, 
//         ALLEGRO_BITMAP* lv5_w, ALLEGRO_BITMAP* lv5_a, 
//         ALLEGRO_BITMAP* lv6_w, ALLEGRO_BITMAP* lv6_a, 
//         ALLEGRO_BITMAP* lv7_w, ALLEGRO_BITMAP* lv7_a, 
//     )
// {
//     weapon_lv2_bitmap = lv1_w;
//     weapon_lv3_bitmap = lv2_w;
//     weapon_lv4_bitmap = lv3_w;
//     weapon_lv5_bitmap = lv4_w;
//     weapon_lv6_bitmap = lv5_w;
//     weapon_lv7_bitmap = lv6_w;

//     arrow_lv2_bitmap = lv1_a;
//     arrow_lv3_bitmap = lv2_a;
//     arrow_lv4_bitmap = lv3_a;
//     arrow_lv5_bitmap = lv4_a;
//     arrow_lv6_bitmap = lv5_a;
//     arrow_lv7_bitmap = lv6_a;
// }

void Weapon::attach(float* hx, float* hy)
{
    host_x = hx;
    host_y = hy;
}

void Weapon::setFireRate(float sec)
{
    fire_interval = sec;
}

void Weapon::setDogType(int type)
{
    dog_type = type;
}

void Weapon::fire()
{
    if (!arrow_img || !host_x || !host_y) return;

    float base_x = *host_x + 50;
    float base_y = *host_y - 280;

    // 根據狗的類型決定箭的屬性
    float speed = 15;
    float scale = 1.0f;
    int arrow_count = 1;
    float vertical_spacing = 50;

    switch(dog_type) {
        case 1:
            speed = 15;
            scale = 1.0f;
            arrow_count = 1;

            fire_interval = 1;
            break;
        case 2:
            speed = 30;
            scale = 1.5f;
            arrow_count = 5;

            fire_interval = .5f;
            break;
        case 3:
            speed = 60;
            scale = 2;
            arrow_count = 10;

            fire_interval = .2f;
            break;
    }

    // 計算起始偏移，讓箭矢以中心對齊
    float start_offset = -(arrow_count - 1) * vertical_spacing / 2.0f;

    // 發射多排箭
    for (int i = 0; i < arrow_count; i++) {
        Arrow a;
        float arrow_y = base_y + start_offset + i * vertical_spacing;
        a.init(base_x, arrow_y, speed, arrow_img, scale);
        arrows.push_back(a);
    }
}

void Weapon::update(float delta)
{
    if (!host_x || !host_y) return;

    fire_timer += delta;

    if (fire_timer >= fire_interval) {
        fire();
        fire_timer = 0;
    }

    for (auto& a : arrows)
        a.update();

    arrows.erase(
        std::remove_if(arrows.begin(), arrows.end(),
                       [](const Arrow& a) { return a.getX() > 1600; }),
        arrows.end()
    );
}

void Weapon::draw()
{
    if (!host_x || !host_y) return;

    if (bow_img) {
        int bow_w = al_get_bitmap_width(bow_img);
        int bow_h = al_get_bitmap_height(bow_img);
        
        float bow_x = *host_x + 70;
        float bow_y = *host_y - 280;
        
        al_draw_scaled_bitmap(
            bow_img, 0, 0, bow_w, bow_h,
            bow_x, bow_y,
            bow_w, bow_h,
            0
        );
    }

    for (auto& a : arrows)
        a.draw();
}