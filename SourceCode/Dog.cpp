// Dog.cpp
#include "Dog.h"
#include "Utils.h"
#include "data/DataCenter.h"
#include "data/ImageCenter.h"
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>   // 如果使用 TTF 字型

constexpr char dog1_img_path[] = "./assets/image/dog1.png";
constexpr char dog2_img_path[] = "./assets/image/dog2.png"; // 30隻dog1
constexpr char dog3_img_path[] = "./assets/image/dog3.png"; // 30隻dog2

void Dog::init() {
    // DataCenter *DC = DataCenter::get_instance();
    ImageCenter *IC = ImageCenter::get_instance();

    dog1_img = IC->get(dog1_img_path);
    dog2_img = IC->get(dog2_img_path);
    dog3_img = IC->get(dog3_img_path);

    
    if (!font) { 
        font = al_load_ttf_font("./assets/font/Caviar_Dreams_Bold.ttf", 32, 0);
        if (!font) font = al_create_builtin_font(); // fallback
    }


    x = 100;
    y = 700;      // 這是底部位置，跟你現在的 dog1_y_bottom 一樣

    num_dogs = 1;

    vy = 0;
    gravity = 0.6;       // 重力（可調）
    jump_speed = -18;    // 跳躍初速度（可調）
    on_ground = true;

    jump_count = 0;   // 一開始沒跳
    max_jump = 2;     // 二段跳
}

// 將 num_dogs 轉換成 dog1/dog2/dog3 的顯示數量
void Dog::splitDogs(int total, int& n3, int& n2, int& n1)
{
    // 最大上限防爆畫
    if (total > 126000)
        total = 126000;

    // 每 30 dog2 = 1 dog3，因此 1 dog3 = 900 dog1
    const int DOG1_PER_DOG2 = 30;
    const int DOG1_PER_DOG3 = 30 * 30;   // = 900

    n3 = total / DOG1_PER_DOG3;
    int remain = total % DOG1_PER_DOG3;

    n2 = remain / DOG1_PER_DOG2;
    remain = remain % DOG1_PER_DOG2;

    n1 = remain;
}




Dog::DogDrawInfo Dog::getDogInfo(int type)
{
    switch (type) {
        case 1: return { dog1_img, 0.4f };
        case 2: return { dog2_img, 0.45f };
        case 3: return { dog3_img, 0.5f };
    }
    return { dog1_img, 0.4f };
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

void Dog::draw() 
{
    int n3, n2, n1;
    splitDogs(num_dogs, n3, n2, n1);

    const float offsetX = 20;     
    const float rowSpacing = 50;  
    const int maxPerRow = 10;     

    int dogIndex = 0;

    auto drawOneDog = [&](ALLEGRO_BITMAP* bmp, float base_scale)
    {
        int w = al_get_bitmap_width(bmp);
        int h = al_get_bitmap_height(bmp);

        double t = al_get_time();
        float y_vibration = 0.95f + 0.05f * sin(t * 2 * ALLEGRO_PI);

        float scaleX = base_scale;
        float scaleY = base_scale * y_vibration;

        int row = dogIndex / maxPerRow;
        int col = dogIndex % maxPerRow;

        float draw_x = x + col * offsetX;
        float draw_h = h * scaleY;
        float draw_y = (y - draw_h) - row * rowSpacing;

        al_draw_scaled_bitmap(
            bmp,
            0, 0, w, h,
            draw_x, draw_y,
            w * scaleX, draw_h,
            0
        );

        dogIndex++;
    };

    for (int i=0;i<n1;i++) drawOneDog(getDogInfo(1).bmp, getDogInfo(1).scale);
    for (int i=0;i<n2;i++) drawOneDog(getDogInfo(2).bmp, getDogInfo(2).scale);
    for (int i=0;i<n3;i++) drawOneDog(getDogInfo(3).bmp, getDogInfo(3).scale);

    // --- 數字顯示 ---
    char buf[16];
    sprintf(buf, "%d", num_dogs);

    al_draw_text(
        font,
        al_map_rgb(0,0,0),
        50, 50,
        ALLEGRO_ALIGN_LEFT,
        buf
    );
}


Dog::~Dog()
{
    if (font) {
        al_destroy_font(font);
        font = nullptr;
    }
}



