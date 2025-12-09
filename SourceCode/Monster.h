// Monster.h
#ifndef MONSTER_H
#define MONSTER_H

#include <allegro5/allegro.h>
#include "algif5/algif.h"

enum class MonsterTYPE
{
    LV1_SLIME,
    LV2,
    LV3,
    LV4,
    LV5
};

class Monster
{
private:
    float x, y;
    float width, height;
    float speed;
    int hp, hp_max;
    MonsterTYPE type;
    ALGIF_ANIMATION* gif;

    // 靜態資源池（所有怪物共用）
    static ALGIF_ANIMATION* gif_lv1;
    static ALGIF_ANIMATION* gif_lv2;
    static ALGIF_ANIMATION* gif_lv3;
    static ALGIF_ANIMATION* gif_lv4;
    static ALGIF_ANIMATION* gif_lv5;

public:
    Monster();
    void init(int startX, int startY, MonsterTYPE t);
    void update();
    void draw();
    
    // 預載入資源（在遊戲開始時呼叫一次）
    static void load_resources();
    static void unload_resources();
    
    float getX() const { return x; }
    float getY() const { return y; }
    float getWidth() const { return width; }
    float getHeight() const { return height; }
    int getHP() const { return hp; }
    void takeDamage(int damage) { hp -= damage; if (hp < 0) hp = 0; }
    bool isDead() const { return hp <= 0; }
};

#endif