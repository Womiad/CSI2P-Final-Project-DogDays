#ifndef WEAPON_H_INCLUDED
#define WEAPON_H_INCLUDED

#include <allegro5/allegro_image.h>
#include <vector>
#include "Arrow.h"

#include <vector>
#include <allegro5/allegro.h>
#include "Arrow.h"
class Weapon {
public:
    void init(ALLEGRO_BITMAP* bowImg, ALLEGRO_BITMAP* arrowImg);
    void attach(float* hx, float* hy);
    void setFireRate(float sec);
    void setDogType(int type);  // 確保有這行
    void update(float delta);
    void draw();

    void setWeaponImage(ALLEGRO_BITMAP* bow, ALLEGRO_BITMAP* arrow);

    // static void setupBitmap(
    //     ALLEGRO_BITMAP* lv1_w, ALLEGRO_BITMAP* lv1_a, 
    //     ALLEGRO_BITMAP* lv2_w, ALLEGRO_BITMAP* lv2_a, 
    //     ALLEGRO_BITMAP* lv3_w, ALLEGRO_BITMAP* lv3_a, 
    //     ALLEGRO_BITMAP* lv4_w, ALLEGRO_BITMAP* lv4_a, 
    //     ALLEGRO_BITMAP* lv5_w, ALLEGRO_BITMAP* lv5_a, 
    //     ALLEGRO_BITMAP* lv6_w, ALLEGRO_BITMAP* lv6_a, 
    //     ALLEGRO_BITMAP* lv7_w, ALLEGRO_BITMAP* lv7_a, 
    // );
    
private:
    void fire();
    
    float* host_x = nullptr;
    float* host_y = nullptr;

    
    ALLEGRO_BITMAP* bow_img = nullptr;
    ALLEGRO_BITMAP* arrow_img = nullptr;

    // static ALLEGRO_BITMAP* weapon_lv2_bitmap;
    // static ALLEGRO_BITMAP* weapon_lv3_bitmap;
    // static ALLEGRO_BITMAP* weapon_lv4_bitmap;
    // static ALLEGRO_BITMAP* weapon_lv5_bitmap;
    // static ALLEGRO_BITMAP* weapon_lv6_bitmap;
    // static ALLEGRO_BITMAP* weapon_lv7_bitmap;

    // static ALLEGRO_BITMAP* arrow_lv2_bitmap;
    // static ALLEGRO_BITMAP* arrow_lv3_bitmap;
    // static ALLEGRO_BITMAP* arrow_lv4_bitmap;
    // static ALLEGRO_BITMAP* arrow_lv5_bitmap;
    // static ALLEGRO_BITMAP* arrow_lv6_bitmap;
    // static ALLEGRO_BITMAP* arrow_lv7_bitmap;
    
    float fire_interval = 1.0f;
    float fire_timer = 0.0f;
    int dog_type = 1;  // 確保有這行
    
    std::vector<Arrow> arrows;
};

#endif