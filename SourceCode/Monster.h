#ifndef MONSTER_H
#define MONSTER_H

#include <allegro5/allegro.h>
#include "algif5/algif.h"

// 前向聲明
class Dog;

class Monster
{
public:
    Monster();
    virtual ~Monster();

    float x, y;
    int width, height;
    float speed;
    int hp, hp_max, base_hp;
    float scale;
    ALGIF_ANIMATION* gif;

    void setHPMultiplier(float multiplier);
    void setHP(int newHP);
    void updateScale();
    
    // ⭐ 新增：設定每秒對玩家造成的傷害
    void setDamagePerSecond(int damage);
    
    // ⭐ 新增：虛擬函數供子類別覆寫
    virtual void init(int startX, int startY);
    virtual void load_gif();
    
    // ⭐ 修改：update 需要傳入玩家指標
    virtual void update(Dog* player = nullptr);
    
    virtual void draw();
    void drawHealthBar();
    
    // ⭐ 新增：getter 方法
    float getX() const { return x; }
    float getY() const { return y; }
    bool isDead() const { return hp <= 0; }

protected:
    // ⭐ 新增成員變數
    int damagePerSecond;        // 每秒對玩家造成的傷害
    float damageTimer;          // 計時器（累積到 1.0 就扣血）
    bool hasReachedTarget;      // 是否已到達攻擊位置
};

#endif