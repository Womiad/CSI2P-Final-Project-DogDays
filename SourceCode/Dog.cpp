// Dog.cpp
#include "Dog.h"
#include "Utils.h"
#include "data/DataCenter.h"
#include "data/ImageCenter.h"
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>

constexpr char dog1_img_path[] = "./assets/image/dog1.png";

void Dog::init() {
    DataCenter *DC = DataCenter::get_instance();
    ImageCenter *IC = ImageCenter::get_instance();

    dog1_img = IC -> get(dog1_img_path);

    x = 100;
    y = 700;      // 這是底部位置，跟你現在的 dog1_y_bottom 一樣

    vy = 0;
    gravity = 0.6;       // 重力（可調）
    jump_speed = -18;    // 跳躍初速度（可調）
    on_ground = true;

    jump_count = 0;   // 一開始沒跳
    max_jump = 2;     // 二段跳
}

void Dog::update() {
    DataCenter *DC = DataCenter::get_instance();

    bool space_now = DC->key_state[ALLEGRO_KEY_SPACE];
    bool space_prev = DC->prev_key_state[ALLEGRO_KEY_SPACE];
    bool just_pressed_space = space_now && !space_prev;

    // --- 跳躍條件：當前跳躍次數 < max_jump ---
    if (just_pressed_space && jump_count < max_jump) {
        vy = jump_speed;
        on_ground = false;
        jump_count++;
    }

    // --- 套用重力 ---
    vy += gravity;
    y += vy;

    // --- 落地判定 ---
    float ground_y = 700;

    if (y > ground_y) {
        y = ground_y;
        vy = 0;
        on_ground = true;
        jump_count = 0;   // ★ 落地後重置跳躍次數
    }
}



void Dog::draw() {
    DataCenter *DC = DataCenter::get_instance();
    if (dog1_img) {
        int dog1_w = al_get_bitmap_width(dog1_img);
        int dog1_h = al_get_bitmap_height(dog1_img);

        float base_scale = 0.4f;  // 整體縮小成 40%

        // --- y 方向 1 ↔ 0.9 的呼吸縮放 ---
        double t = al_get_time();
        float y_vibration = 0.95f + 0.05f * sin(t * 2 * ALLEGRO_PI);  // 0.9~1.0
        float xscale = base_scale;
        float yscale = base_scale * y_vibration;  // 在 0.45~0.5 之間變化

        int dog1_x = 100;
        int dog1_y_bottom = y;   // 你希望底部固定的位置

        // ---- 底部固定：根據縮放調整 y ----
        float scaled_height = dog1_h * yscale;
        float draw_y = dog1_y_bottom - scaled_height;  // 往上縮放

        al_draw_scaled_bitmap(
            dog1_img,
            0, 0, dog1_w, dog1_h,
            dog1_x, draw_y,
            dog1_w * xscale, scaled_height,
            0
        );
    }
}



