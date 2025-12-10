#ifndef MONSTER_H
#define MONSTER_H

#include <allegro5/allegro.h>
#include "algif5/algif.h"

class Monster
{
protected:
    float x, y;
    float width, height;
    float speed;
    int hp, hp_max;
    int base_hp;
    float scale;  // ⭐ 新增：縮放比例
    
    ALGIF_ANIMATION* gif;

public:
    Monster();
    virtual ~Monster();

    virtual void init(int startX, int startY) = 0;
    virtual void load_gif() = 0;
    
    void update();
    void draw();
    
    void setHPMultiplier(float multiplier);
    void setHP(int newHP);
    
    // ⭐ 新增：更新縮放比例
    void updateScale();
    
    float getX() const { return x; }
    float getY() const { return y; }
    float getWidth() const { return width * scale; }  // ⭐ 考慮縮放
    float getHeight() const { return height * scale; }
    int getHP() const { return hp; }
    int getMaxHP() const { return hp_max; }
    int getBaseHP() const { return base_hp; }
    bool isDead() const { return hp <= 0; }
    
    void takeDamage(int damage) { hp -= damage; if (hp < 0) hp = 0; }

protected:
    void drawHealthBar();
};

#endif