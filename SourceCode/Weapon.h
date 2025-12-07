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
    
private:
    void fire();
    
    ALLEGRO_BITMAP* bow_img = nullptr;
    ALLEGRO_BITMAP* arrow_img = nullptr;
    float* host_x = nullptr;
    float* host_y = nullptr;
    
    float fire_interval = 1.0f;
    float fire_timer = 0.0f;
    int dog_type = 1;  // 確保有這行
    
    std::vector<Arrow> arrows;
};

#endif