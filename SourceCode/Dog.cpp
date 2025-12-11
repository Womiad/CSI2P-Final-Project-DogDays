// Dog.cpp
#include "Dog.h"
#include "Utils.h"
#include "data/DataCenter.h"
#include "data/ImageCenter.h"
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

#include <iostream>

constexpr char dog1_img_path[] = "./assets/image/dog1.png";
constexpr char dog2_img_path[] = "./assets/image/dog2.png";
constexpr char dog3_img_path[] = "./assets/image/dog3.png";

constexpr char weapon_path[] = "./assets/image/bow.png";
constexpr char arrow_path[]  = "./assets/image/arrow.png";

constexpr char weapon2_img_path[] = "./assets/image/weapon/weapon_lv2.png";
constexpr char weapon3_img_path[] = "./assets/image/weapon/weapon_lv3.png";
constexpr char weapon4_img_path[] = "./assets/image/weapon/weapon_lv4.png";
constexpr char weapon5_img_path[] = "./assets/image/weapon/weapon_lv5.png";
constexpr char weapon6_img_path[] = "./assets/image/weapon/weapon_lv6.png";
constexpr char weapon7_img_path[] = "./assets/image/weapon/weapon_lv7.png";

constexpr char arrow2_img_path[] = "./assets/image/arrow.png";
constexpr char arrow3_img_path[] = "./assets/image/weapon/arrow_lv3_lv4.png";
constexpr char arrow4_img_path[] = "./assets/image/weapon/arrow_lv3_lv4.png";
constexpr char arrow5_img_path[] = "./assets/image/weapon/arrow_lv5.png";
constexpr char arrow6_img_path[] = "./assets/image/weapon/arrow_lv6.png";
constexpr char arrow7_img_path[] = "./assets/image/weapon/arrow_lv7.png";

Dog::Dog() : x(100), y(700), num_dogs(1),
             vy(0), gravity(0.6f), jump_speed(-18),
             on_ground(true), jump_count(0), max_jump(2),
             prev_num_dogs(0)  // 新增：追蹤上次的狗數量
{
    w = 300;
    h = 288;
}

Dog::~Dog()
{
    if (font) {
        al_destroy_font(font);
        font = nullptr;
    }
}

void Dog::init()
{
    ImageCenter* IC = ImageCenter::get_instance();

    dog1_img = IC->get(dog1_img_path);
    dog2_img = IC->get(dog2_img_path);
    dog3_img = IC->get(dog3_img_path);

    bow_img   = IC->get(weapon_path);
    arrow_img = IC->get(arrow_path);

    weapon_lv1_bitmap = IC->get(weapon_path);
    weapon_lv2_bitmap = IC->get(weapon2_img_path);
    weapon_lv3_bitmap = IC->get(weapon3_img_path);
    weapon_lv4_bitmap = IC->get(weapon4_img_path);
    weapon_lv5_bitmap = IC->get(weapon5_img_path);
    weapon_lv6_bitmap = IC->get(weapon6_img_path);
    weapon_lv7_bitmap = IC->get(weapon7_img_path);

    arrow_lv1_bitmap = IC->get(arrow_path);
    arrow_lv2_bitmap = IC->get(arrow_path);
    arrow_lv3_bitmap = IC->get(arrow3_img_path);
    arrow_lv4_bitmap = IC->get(arrow4_img_path);
    arrow_lv5_bitmap = IC->get(arrow5_img_path);
    arrow_lv6_bitmap = IC->get(arrow6_img_path);
    arrow_lv7_bitmap = IC->get(arrow7_img_path);

    if (!font) { 
        font = al_load_ttf_font("./assets/font/Caviar_Dreams_Bold.ttf", 32, 0);
        if (!font) font = al_create_builtin_font();
    }
}

void Dog::splitDogs(int total, int& n3, int& n2, int& n1)
{
    const int DOG1_PER_DOG2 = 30;
    const int DOG1_PER_DOG3 = 30 * 30; // 900

    if (total > 126000)
        total = 126000;

    n3 = total / DOG1_PER_DOG3;
    int remain = total % DOG1_PER_DOG3;

    n2 = remain / DOG1_PER_DOG2;
    remain = remain % DOG1_PER_DOG2;

    n1 = remain;

    refreshDogUnits(n1, n2, n3);
}

void Dog::refreshDogUnits(int n1, int n2, int n3)
{
    dogs.clear();

    // 先預留空間，避免 vector 重新分配記憶體
    int total = n1 + n2 + n3;
    dogs.reserve(total);

    auto createDog = [&](int type, float offsetX, float offsetY){
        DogUnit d;
        d.offset_x = offsetX;
        d.offset_y = offsetY;
        d.x = x + offsetX;
        d.y = y + offsetY;
        d.type = type;

        d.weapon.init(bow_img, arrow_img);
        d.weapon.setDogType(type);  // 設定狗的類型

        // 射擊速度依狗種調整
        if (type == 1)      d.weapon.setFireRate(1.0f);
        else if (type == 2) d.weapon.setFireRate(0.7f);
        else if (type == 3) d.weapon.setFireRate(0.4f);

        dogs.push_back(d);
        
        // 關鍵修正：在 push_back 之後，綁定 vector 內部的位址
        dogs.back().weapon.attach(&dogs.back().x, &dogs.back().y);
    };

    const float offsetX = 30;  // 增加間距讓狗不會重疊
    const float rowSpacing = 80;
    const int maxPerRow = 10;

    int idx = 0;

    auto batch = [&](int count, int type){
        for (int i = 0; i < count; i++) {
            int row = idx / maxPerRow;
            int col = idx % maxPerRow;

            float ox = col * offsetX;
            float oy = -row * rowSpacing;

            createDog(type, ox, oy);
            idx++;
        }
    };

    batch(n1, 1);
    batch(n2, 2);
    batch(n3, 3);
}

Dog::DogDrawInfo Dog::getDogInfo(int type)
{
    switch (type) {
        case 1: return { dog1_img, 0.40f };
        case 2: return { dog2_img, 0.55f };
        case 3: return { dog3_img, 0.70f };
    }
    return { dog1_img, 0.4f };
}

void Dog::update()
{
    DataCenter* DC = DataCenter::get_instance();

    bool space_now  = DC->key_state[ALLEGRO_KEY_SPACE];
    bool space_prev = DC->prev_key_state[ALLEGRO_KEY_SPACE];

    if (space_now && !space_prev && jump_count < max_jump) {
        vy = jump_speed;
        on_ground = false;
        jump_count++;
    }

    vy += gravity;
    y += vy;

    float ground_y = 700;

    if (y > ground_y) {
        y = ground_y;
        vy = 0;
        on_ground = true;
        jump_count = 0;
    }

    // 只在狗的數量改變時才重建陣列
    if (num_dogs != prev_num_dogs) {
        int n3, n2, n1;
        splitDogs(num_dogs, n3, n2, n1);
        prev_num_dogs = num_dogs;
    }

    // 更新每隻狗的絕對位置並更新武器
    for (auto& d : dogs) {
        d.x = x + d.offset_x;
        d.y = y + d.offset_y;
        
        // 關鍵：確保武器有被更新
        d.weapon.update(1.0f / 60.0f);
    }

    // 更新DataCenter數據
    DC -> nowScore = num_dogs;

    if(num_dogs > DC->playerHighestScore) DC -> playerHighestScore = num_dogs;

    if(DC -> nowScore >= 10 && DC -> nowScore < 50){
        if(DC -> idelWeaponLevel < 2) DC -> idelWeaponLevel = 2;
    }else if(DC -> nowScore >= 50 && DC -> nowScore < 300){
        if(DC -> idelWeaponLevel < 3) DC -> idelWeaponLevel = 3;
    }else if(DC -> nowScore >= 300 && DC -> nowScore < 1500){
        if(DC -> idelWeaponLevel < 4) DC -> idelWeaponLevel = 4;
    }else if(DC -> nowScore >= 1500 && DC -> nowScore < 9000){
        if(DC -> idelWeaponLevel < 5) DC -> idelWeaponLevel = 5;
    }else if(DC -> nowScore >= 9000 && DC -> nowScore < 45000){
        if(DC -> idelWeaponLevel < 6) DC -> idelWeaponLevel = 6;
    }else if(DC -> nowScore >= 9000 && DC -> nowScore < 2250000){
        if(DC -> idelWeaponLevel < 7) DC -> idelWeaponLevel = 7;
    }else if(DC -> nowScore >= 2250000){
        if(DC -> idelWeaponLevel < 8) DC -> idelWeaponLevel = 8;
    }else{
        DC -> idelWeaponLevel = 1;
    }

    switch (DC -> weaponLevel)
    {
    case 1:
        bow_img = weapon_lv1_bitmap;
        arrow_img = arrow_lv1_bitmap;
        break;
    case 2:
        bow_img = weapon_lv2_bitmap;
        arrow_img = arrow_lv2_bitmap;
        break;
    case 3:
        bow_img = weapon_lv3_bitmap;
        arrow_img = arrow_lv3_bitmap;
        break;
    case 4:
        bow_img = weapon_lv4_bitmap;
        arrow_img = arrow_lv4_bitmap;
        break;
    case 5:
        bow_img = weapon_lv5_bitmap;
        arrow_img = arrow_lv5_bitmap;
        break;
    case 6:
        bow_img = weapon_lv6_bitmap;
        arrow_img = arrow_lv6_bitmap;
        break;
    case 7:
        bow_img = weapon_lv7_bitmap;
        arrow_img = arrow_lv7_bitmap;
        break;
    
    default:
        break;
    }

    for (auto& d : dogs) {
        d.weapon.setWeaponImage(bow_img, arrow_img);
    }


    // std::cout << "score: " << DC -> nowScore << std::endl;
    // std::cout << "idelWeaponLevel: " << DC -> idelWeaponLevel << std::endl;
}

void Dog::draw()
{
    // 不要在這裡呼叫 splitDogs！
    // splitDogs 會清空 dogs vector，導致所有武器消失
    // 應該只在 num_dogs 改變時才呼叫

    // 畫每隻狗和武器
    for (auto& d : dogs) {
        auto info = getDogInfo(d.type);

        int img_w = al_get_bitmap_width(info.bmp);
        int img_h = al_get_bitmap_height(info.bmp);

        float draw_w = img_w * info.scale;
        float draw_h = img_h * info.scale;

        // 繪製狗（位置在 d.x, d.y - draw_h）
        al_draw_scaled_bitmap(
            info.bmp, 0, 0, img_w, img_h,
            d.x, d.y - draw_h,
            draw_w, draw_h,
            0
        );

        // 繪製武器（會在狗的頭部附近）
        d.weapon.draw();
    }

    // // === 繪製碰撞箱（紅框） ===
    // al_draw_rectangle(
    //     x,                // 左
    //     y - h,       // 上
    //     x + w,       // 右
    //     y,                // 下
    //     al_map_rgb(255, 0, 0),
    //     2
    // );


    // 顯示數字
    char buf[32];
    sprintf(buf, "%d", num_dogs);

    al_draw_text(font, al_map_rgb(0,0,0), 50, 50, 0, buf);
}